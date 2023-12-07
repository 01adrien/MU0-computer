#ifndef _SCANNER_H_
#define _SCANNER_H_

#define MAX_TOKEN 3

#include <stdbool.h>

typedef struct token {
    char* start;
    int lenght;
} token_t;

typedef struct scanner {
    char* source;
} scanner_t;

void
skip_blanks(scanner_t* scanner);

bool
end_symbol(scanner_t* scanner);

bool
end_line(scanner_t* scanner);

bool
end_file(scanner_t* scanner);

token_t
tokenize(scanner_t* scanner);

void
print_tokens(token_t* tokens, int size);

#endif