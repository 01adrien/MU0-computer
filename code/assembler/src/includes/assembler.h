#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include "../../../const.h"

#include "lexer.h"
#include "parser.h"

static uint8_t opcode_table[15] = {
    [TOKEN_LDA] = 0b0000, [TOKEN_STR] = 0b0001, [TOKEN_ADD] = 0b0010,
    [TOKEN_SUB] = 0b0011, [TOKEN_JMP] = 0b0100, [TOKEN_LDI] = 0b0101,
    [TOKEN_JEZ] = 0b0110, [TOKEN_OR] = 0b0111,  [TOKEN_NOT] = 0b1000,
    [TOKEN_JGZ] = 0b1001, [TOKEN_JLZ] = 0b1010, [TOKEN_LEA] = 0b1011,
    [TOKEN_STA] = 0b1100, [TOKEN_AND] = 0b1101, [TOKEN_HLT] = 0b1111,
};

typedef struct symbols symbols_t;

struct symbols {
    char* id;
    uint32_t address;
    symbols_t* next;
};

typedef struct assembler {
    char* file_name;
    char** object_file;
    symbols_t* symbols_table;
    bool is_error;
    uint32_t IC;
    uint32_t AC;
} assembler_t;

void
init_assembler(char* file_name, assembler_t* assembler, uint32_t AC);

void
run_assembler(assembler_t* assembler, token_queue_t** queues);

void
print_object_file(char** object_file);

#endif // !1