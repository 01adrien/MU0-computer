#ifndef _LEXER_H_
#define _LEXER_H_

#include "common.h"
#include "scanner.h"

typedef enum token_type {
    TOKEN_LDA,
    TOKEN_STR,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_JMP,
    TOKEN_JLZ,
    TOKEN_JEZ,
    TOKEN_JGZ,
    TOKEN_HLT,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_LDI,
    TOKEN_LEA,
    TOKEN_STA,
    TOKEN_AND,
    TOKEN_NEG,
    TOKEN_EOF,
    TOKEN_DATA,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_LABEL,
    TOKEN_ERROR,
    TOKEN_NEWLINE,
} token_type_t;

typedef struct token {
    token_type_t type;
    const char* start;
    int length;
    int line;
    uint64_t addr;
} token_t;

typedef struct lexer {
    token_t previous;
    token_t current;
    token_t next;
} lexer_t;

#define PRINT_TOKEN(token)                                                     \
    printf("[TOKEN %s] %.*s\n", type(token), (token).length, (token).start)

token_t
scan_token(scanner_t* scanner);

void
advance_lexer(lexer_t* lexer, scanner_t* scanner);

char*
type(token_t token);

token_t
make_token(token_type_t type, scanner_t* scanner);

token_t
create_token(token_type_t type, const char* start, int length);

#endif