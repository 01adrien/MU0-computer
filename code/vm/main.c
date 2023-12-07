#include "src/includes/scanner.h"
#include "src/includes/translator.h"
#include "src/includes/utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char const* argv[]) {

    scanner_t scanner;
    char* source = read_file(argv[1]);
    char* file_name = strtok((char*)argv[1], ".");
    scanner.source = source;
    char* asm_file = parse_all(&scanner);
    // printf("%s\n", asm_file);
    write_to_file(asm_file, file_name);
    free(source);
}
