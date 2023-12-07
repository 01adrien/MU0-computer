#include "includes/lexer.h"

token_t
token_number(scanner_t* scanner);

token_t
token_error(const char* msg, scanner_t* scanner);

token_t
token_identifier(scanner_t* scanner);

token_t
token_label(scanner_t* scanner);

token_type_t
is_reserved(scanner_t* scanner);

bool
match_str(scanner_t* scanner, char* str, int len);

bool
is_special_char(scanner_t* scanner);

token_type_t
is_reserved(scanner_t* scanner) {
    token_type_t type;
    if (match_str(scanner, "lda", 3))
        type = TOKEN_LDA;
    else if (match_str(scanner, "str", 3))
        type = TOKEN_STR;
    else if (match_str(scanner, "add", 3))
        type = TOKEN_ADD;
    else if (match_str(scanner, "sub", 3))
        type = TOKEN_SUB;
    else if (match_str(scanner, "jmp", 3))
        type = TOKEN_JMP;
    else if (match_str(scanner, "jlz", 3))
        type = TOKEN_JLZ;
    else if (match_str(scanner, "jez", 3))
        type = TOKEN_JEZ;
    else if (match_str(scanner, "jgz", 3))
        type = TOKEN_JGZ;
    else if (match_str(scanner, "hlt", 3))
        type = TOKEN_HLT;
    else if (match_str(scanner, "or", 2))
        type = TOKEN_OR;
    else if (match_str(scanner, "not", 3))
        type = TOKEN_NOT;
    else if (match_str(scanner, "ldi", 3))
        type = TOKEN_LDI;
    else if (match_str(scanner, "lea", 3))
        type = TOKEN_LEA;
    else if (match_str(scanner, "sta", 3))
        type = TOKEN_STA;
    else if (match_str(scanner, "and", 3))
        type = TOKEN_AND;
    else if (match_str(scanner, "data", 4))
        type = TOKEN_DATA;
    else if (match_str(scanner, "neg", 3))
        type = TOKEN_NEG;
    else
        type = TOKEN_IDENTIFIER;
    return type;
}

token_t
scan_token(scanner_t* scanner) {
    skip_blanks(scanner);
    scanner->start = scanner->current;
    if (end_file(scanner)) {
        return make_token(TOKEN_EOF, scanner);
    }
    char c = advance_scanner(scanner);
    if (isdigit(c)) {
        return token_number(scanner);
    }
    if (isalpha(c)) {
        return token_identifier(scanner);
    }
    switch (c) {
        case '\n':
            scanner->line++;
            return make_token(TOKEN_NEWLINE, scanner);
        case '.':
            return token_label(scanner);
        case '-':
            advance_scanner(scanner);
            return token_number(scanner);
        default:
            printf("[ %c ]\n", c);
            return token_error("unknow token.", scanner);
    }
}

token_t
token_number(scanner_t* scanner) {
    while (isdigit(peek_scanner(scanner))) {
        advance_scanner(scanner);
    }
    return make_token(TOKEN_NUMBER, scanner);
}

token_t
token_identifier(scanner_t* scanner) {
    while (isalpha(peek_scanner(scanner)) || isdigit(peek_scanner(scanner)) ||
           is_special_char(scanner)) {
        advance_scanner(scanner);
    }
    return make_token(is_reserved(scanner), scanner);
}

token_t
token_label(scanner_t* scanner) {
    advance_scanner(scanner);
    while (isalpha(peek_scanner(scanner)) || isdigit(peek_scanner(scanner)) ||
           is_special_char(scanner)) {
        advance_scanner(scanner);
    }
    return make_token(TOKEN_LABEL, scanner);
}

token_t
make_token(token_type_t type, scanner_t* scanner) {
    token_t token;
    token.start = scanner->start;
    token.length = (scanner->current - scanner->start);
    token.type = type;
    token.line = scanner->line;
    return token;
}

token_t
create_token(token_type_t type, const char* start, int length) {
    token_t token;
    token.start = start;
    token.length = length;
    token.type = type;
    token.line = -1;
    return token;
}

token_t
token_error(const char* msg, scanner_t* scanner) {
    token_t token;
    token.start = msg;
    token.length = strlen(msg);
    token.type = TOKEN_ERROR;
    token.line = scanner->line;
    return token;
}

void
advance_lexer(lexer_t* lexer, scanner_t* scanner) {
    lexer->previous = lexer->current;
    lexer->current = lexer->next;
    lexer->next = scan_token(scanner);
}

char*
type(token_t token) {
    switch (token.type) {
        case TOKEN_EOF:
            return "EOF";
        case TOKEN_ERROR:
            return "ERROR";
        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_DATA:
            return "DATA";
        case TOKEN_LDA:
        case TOKEN_STR:
        case TOKEN_ADD:
        case TOKEN_SUB:
        case TOKEN_JMP:
        case TOKEN_JLZ:
        case TOKEN_JEZ:
        case TOKEN_JGZ:
        case TOKEN_HLT:
        case TOKEN_OR:
        case TOKEN_NOT:
        case TOKEN_LEA:
        case TOKEN_STA:
        case TOKEN_AND:
        case TOKEN_NEG:
            return "OPCODE";
        case TOKEN_NEWLINE:
            return "NEW_LINE";
        case TOKEN_LABEL:
            return "LABEL";
        default:
            return "UNKNOW";
    }
}

bool
match_str(scanner_t* scanner, char* str, int len) {
    return memcmp(scanner->start, str, len) == 0;
}

bool
is_special_char(scanner_t* scanner) {
    return match(scanner, '#');
}