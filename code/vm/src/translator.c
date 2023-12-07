#include "includes/translator.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
parse_binary(token_t op);

void
parse_unary(token_t op);

void
parse_push(token_t segment, token_t value);

void
parse_pop(token_t segment, token_t value);

void
process_tokens(token_t* tokens, int size);

void
parse_compare(token_t token);

void
write_vm_code(scanner_t* scanner);

void
write_asm(const char* format, ...);

void
write_push();

void
write_pop();

void
write_get_pointer(char* segment, int index);

bool
match(token_t token, char* string);

bool
is_binary(token_t token);

bool
is_unary(token_t token);

bool
is_compare(token_t token);

static int cmp_id = 0;

char asm_file[1000];

char*
parse_all(scanner_t* scanner) {
    while (!end_file(scanner)) {
        parse_line(scanner);
        scanner->source++;
    }
    write_asm("\nhlt 0\n");
    return asm_file;
}

void
parse_line(scanner_t* scanner) {
    token_t tokens[MAX_TOKEN];
    int size = 0;
    write_vm_code(scanner);
    while (!end_line(scanner)) {
        tokens[size++] = tokenize(scanner);
    }
    if (size) {
        process_tokens(tokens, size);
    }
}

void
parse_binary(token_t op) {
    write_pop();
    write_asm("str R0\n");
    write_pop();
    write_asm("%.*s R0\n", op.lenght, op.start);
    write_push();
}

void
parse_unary(token_t op) {
    write_pop();
    write_asm("str R0\n");
    write_asm("not R0\n");
    if (match(op, "neg")) {
        write_asm("add ONE\n");
    }
    write_push();
}

void
parse_push(token_t segment, token_t value) {
    int index = strtod(value.start, NULL);
    if (match(segment, "constant")) {
        write_asm("ldi %d\n", index);
    } else if (match(segment, "args")) {
        write_get_pointer("ARGS", index);
        write_asm("lea R0\n");
    } else if (match(segment, "local")) {
        write_get_pointer("LOCAL", index);
        write_asm("lea R0\n");
    } else if (match(segment, "temp")) {
        index += TEMP;
        write_asm("ldi %d\n", index);
    }
    write_push();
}

void
parse_pop(token_t segment, token_t value) {
    int index = strtod(value.start, NULL);
    if (match(segment, "args")) {
        write_get_pointer("ARGS", index);
    } else if (match(segment, "local")) {
        write_get_pointer("LOCAL", index);
    }
    write_pop();
    write_asm("sta R0\n");
}

void
parse_compare(token_t token) {
    write_pop();
    write_asm("str R0\n");
    write_pop();
    write_asm("str R1\n");
    write_asm("lda R0\n");
    write_asm("sub R1\n");
    if (match(token, "gt")) {
        write_asm("jgz true#%d\n", cmp_id);
    } else if (match(token, "eq")) {
        write_asm("jez true#%d\n", cmp_id);
    } else if (match(token, "lt")) {
        write_asm("jlz true#%d\n", cmp_id);
    }
    write_asm("ldi 0\n");
    write_asm("jmp end#%d\n", cmp_id);
    write_asm(".true#%d\n", cmp_id);
    write_asm("ldi 1\n");
    write_asm(".end#%d\n", cmp_id);
    write_push();
    cmp_id++;
}

void
process_tokens(token_t* tokens, int size) {
    if (is_binary(tokens[0])) {
        parse_binary(tokens[0]);
    } else if (match(tokens[0], "push")) {
        parse_push(tokens[1], tokens[2]);
    } else if (is_unary(tokens[0])) {
        parse_unary(tokens[0]);
    } else if (is_compare(tokens[0])) {
        parse_compare(tokens[0]);
    } else if (match(tokens[0], "pop")) {
        parse_pop(tokens[1], tokens[2]);
    }
    {}
}

void
write_asm(const char* format, ...) {
    char line[ASM_MAX_LINE];
    sprintf(line, "%s", format);
    va_list args;
    va_start(args, format);
    vsprintf(asm_file + strlen(asm_file), line, args);
    va_end(args);
}

void
write_vm_code(scanner_t* scanner) {
    int c = 0;
    write_asm("\n; ");
    while (!end_line(scanner)) {
        write_asm("%c", *scanner->source);
        scanner->source++;
        c++;
    }
    write_asm("\n");
    scanner->source -= c;
}

void
write_push() {
    write_asm("sta SP\n");
    write_asm("lda SP\n");
    write_asm("add ONE\n");
    write_asm("str SP\n");
}

void
write_pop() {
    write_asm("lda SP\n");
    write_asm("sub ONE\n");
    write_asm("str SP\n");
    write_asm("lea SP\n");
}

void
write_get_pointer(char* segment, int index) {
    write_asm("ldi %d\n", index);
    write_asm("str R0\n");
    write_asm("lda %s\n", segment);
    write_asm("add R0\n");
    write_asm("str R0\n");
}

bool
match(token_t token, char* string) {
    return strncmp(token.start, string, token.lenght) == 0;
}

bool
is_binary(token_t token) {
    return match(token, "add") || match(token, "sub") || match(token, "or") ||
           match(token, "and");
}

bool
is_unary(token_t token) {
    return match(token, "neg") || match(token, "not");
}

bool
is_compare(token_t token) {
    return match(token, "eq") || match(token, "lt") || match(token, "gt");
}