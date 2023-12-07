#include "includes/scanner.h"
#include <stdio.h>

token_t
tokenize(scanner_t* scanner) {
    token_t t;
    skip_blanks(scanner);
    t.start = scanner->source;
    t.lenght = 0;
    while (!end_symbol(scanner)) {
        scanner->source++;
        t.lenght++;
    }
    skip_blanks(scanner);
    return t;
}

void
print_tokens(token_t* tokens, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        token_t t = tokens[i];
        printf("%.*s", t.lenght, t.start);
        printf("%s", i + 1 == size ? "" : ", ");
    }
    printf(" ]\n");
}

void
skip_blanks(scanner_t* scanner) {
    while (*scanner->source == ' ') {
        scanner->source++;
        if (*scanner->source == ';') {
            while (*scanner->source != '\n') {
                scanner->source++;
            }
        }
    }
}

bool
end_symbol(scanner_t* scanner) {
    return !(*scanner->source != ' ' && *scanner->source != '\n' &&
             *scanner->source != '\0');
}

bool
end_line(scanner_t* scanner) {
    return !(*scanner->source != '\n' && *scanner->source != '\0');
}

bool
end_file(scanner_t* scanner) {
    return !*scanner->source != '\0';
}
