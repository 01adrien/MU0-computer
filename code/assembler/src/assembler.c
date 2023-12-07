#include "includes/assembler.h"
#include "includes/utils.h"
#include <stdarg.h>

void
write_object_file(assembler_t* assembler);

char**
init_object_file();

void
assemble_symbols(assembler_t* assembler, token_queue_t* symbols);

void
assemble_instructions(assembler_t* assembler, token_queue_t* instructions);

void
assemble_labels(assembler_t* assembler, token_queue_t* labels);

void
add_symbol(assembler_t* assembler, char* id, uint32_t address);

void
print_symbols(symbols_t* table);

bool
symbols_lookup(symbols_t* table, char* id, uint32_t* address);

char*
get_id(token_t token);

uint32_t
get_number(token_t token);

void
init_constant(assembler_t* assembler);

void
init_assembler(char* file_name, assembler_t* assembler, uint32_t AC) {
    assembler->IC = INSTRUCTION;
    assembler->AC = AC;
    assembler->is_error = false;
    assembler->file_name = file_name;
    assembler->object_file = init_object_file();
    init_constant(assembler);
}

void
run_assembler(assembler_t* assembler, token_queue_t** queues) {
    assemble_labels(assembler, queues[2]);
    assemble_symbols(assembler, queues[1]);
    assemble_instructions(assembler, queues[0]);
    write_object_file(assembler);
    print_symbols(assembler->symbols_table);
    free(queues);
}

char**
init_object_file() {
    char** object_file = calloc(MAX_FILE, sizeof(char*));
    for (size_t i = 0; i < MAX_FILE; i++) {
        char* cell = calloc(8, sizeof(char));
        strcpy(cell, "0000000");
        object_file[i] = cell;
    }
    return object_file;
}

void
assemble_symbols(assembler_t* assembler, token_queue_t* symbols) {
    node_t* current = peek_queue(symbols);
    while (current) {
        token_t token_id = get_token(dequeue_token(symbols));
        char* id = get_id(token_id);
        token_t token_value = get_token(dequeue_token(symbols));
        uint32_t a = 0;
        if (!symbols_lookup(assembler->symbols_table, id, &a)) {
            add_symbol(assembler, id, assembler->AC);
            sprintf(assembler->object_file[assembler->AC++], "%x",
                    get_number(token_value));
        }
        current = peek_queue(symbols);
    }
    free(symbols);
}

void
assemble_instructions(assembler_t* assembler, token_queue_t* instructions) {
    node_t* current = peek_queue(instructions);
    while (current) {
        char instruction_bin[17];
        char* id;
        token_t token_op = get_token(dequeue_token(instructions));
        uint8_t opcode = opcode_table[token_op.type];
        token_t token_val = get_token(dequeue_token(instructions));
        uint32_t address = 0;
        if (token_val.type == TOKEN_IDENTIFIER) {
            id = get_id(token_val);
            bool found = symbols_lookup(assembler->symbols_table, id, &address);
            if (!found) {
                printf("unbound symbol '%s'\n", id);
                free(id);
                exit(0);
            }
            free(id);
        } else {
            address = get_number(token_val);
        }
        char* bin_opcode = int_to_bin(opcode, 4);
        char* bin_address = int_to_bin(address, 28);
        strcpy(instruction_bin, bin_opcode);
        strcat(instruction_bin, bin_address);
        sprintf(assembler->object_file[assembler->IC++], "%x",
                (int)strtol(instruction_bin, NULL, 2));
        current = peek_queue(instructions);
    }
    free(instructions);
}

void
assemble_labels(assembler_t* assembler, token_queue_t* labels) {
    node_t* current = labels->head;
    while (current) {
        token_t token_label = get_token(dequeue_token(labels));
        char* id = get_id(token_label);
        id++;
        add_symbol(assembler, id, token_label.addr);
        current = peek_queue(labels);
    }
    free(labels);
}

void
add_symbol(assembler_t* assembler, char* id, uint32_t address) {
    symbols_t* new = calloc(1, sizeof(symbols_t));
    new->address = address;
    new->id = id;
    new->next = assembler->symbols_table;
    assembler->symbols_table = new;
}

void
print_symbols(symbols_t* table) {
    symbols_t* temp = table;
    while (temp->next) {
        printf("%s = at 0x%x\n", temp->id, temp->address);
        temp = temp->next;
    }
}

bool
symbols_lookup(symbols_t* table, char* id, uint32_t* address) {
    symbols_t* temp = table;
    while (temp) {
        if (strcmp(temp->id, id) == 0) {
            *address = temp->address;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void
print_object_file(char** object_file) {
    for (size_t i = 1; i < MAX_FILE - 1; i++) {
        int z = 4 - strlen(object_file[i - 1]);
        for (int i = z; i > 0; i--) {
            printf("0");
        }
        printf("%s ", object_file[i - 1]);
        if (i % 10 == 0) {
            printf("\n");
        }
    }
}

uint32_t
get_number(token_t token) {
    return strtod(token.start, NULL);
}

char*
get_id(token_t token) {
    char* id = calloc(token.length, 1);
    memcpy(id, token.start, token.length);
    return id;
}

void
write_object_file(assembler_t* assembler) {
    FILE* fptr;
    char* name = malloc(strlen(assembler->file_name + 2));
    strcpy(name, assembler->file_name);
    strcat(name, ".o");
    fptr = fopen(name, "w");
    for (size_t i = 1; i < MAX_FILE; i++) {
        int z = 8 - strlen(assembler->object_file[i - 1]);
        for (int i = z; i > 0; i--) {
            fprintf(fptr, "0");
        }
        fprintf(fptr, "%s ", assembler->object_file[i - 1]);
        free(assembler->object_file[i - 1]);
        if (i % 8 == 0) {
            fprintf(fptr, "\n");
        }
    }
    fclose(fptr);
    free(assembler->object_file);
    free(name);
}

void
init_constant(assembler_t* assembler) {
    symbols_t* table = calloc(1, sizeof(symbols_t));
    table->next = NULL;
    assembler->symbols_table = table;

    add_symbol(assembler, "SP", STACK_P);
    sprintf(assembler->object_file[STACK_P], "%x", STACK);

    add_symbol(assembler, "R0", R0);
    sprintf(assembler->object_file[R0], "%x", 0);

    add_symbol(assembler, "R1", R1);
    sprintf(assembler->object_file[R1], "%x", 0);

    add_symbol(assembler, "R2", R2);
    sprintf(assembler->object_file[R2], "%x", 0);

    add_symbol(assembler, "ZERO", ZERO);
    sprintf(assembler->object_file[ZERO], "%x", 0);

    add_symbol(assembler, "ONE", ONE);
    sprintf(assembler->object_file[ONE], "%x", 1);

    add_symbol(assembler, "LOCAL", LOCAL_P);
    sprintf(assembler->object_file[LOCAL_P], "%x", LOCAL);

    add_symbol(assembler, "ARGS", ARGS_P);
    sprintf(assembler->object_file[ARGS_P], "%x", ARGS);

    add_symbol(assembler, "THIS", THIS_P);
    sprintf(assembler->object_file[THIS_P], "%x", 0);

    add_symbol(assembler, "THAT", THAT_P);
    sprintf(assembler->object_file[THAT_P], "%x", 0);

    /*
    for (size_t i = 0; i < 3; i++) {
        sprintf(assembler->object_file[ARGS + i], "%x", i + 1);
        sprintf(assembler->object_file[LOCAL + i], "%x", i + 10);
        sprintf(assembler->object_file[TEMP + i], "%x", i + 20);
        sprintf(assembler->object_file[STATIC + i], "%x", i + 30);
    }
    */
}