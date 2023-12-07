#include "src/includes/assembler.h"
#include "src/includes/lexer.h"
#include "src/includes/parser.h"
#include "src/includes/scanner.h"
#include "src/includes/utils.h"
#include <string.h>

int
main(int argc, char const* argv[]) {
    scanner_t scanner;
    lexer_t lexer;
    parser_t parser;
    assembler_t assembler;

    char* command = malloc(8 + strlen(argv[1]));
    sprintf(command, "cat -n %s", argv[1]);
    system(command);
    free(command);
    printf("\n");

    char* source = read_file(argv[1]);
    char* file_name = strtok((char*)argv[1], ".");
    init_scanner(source, &scanner);
    init_parser(&lexer, &scanner, &parser);
    parse_error_t parse_error = run_parser(&parser);
    if (parser.is_error) {
        printf("Error: %s at line %d\n", parse_error, parser.scanner->line);
        // clean exit please !!
    } else {
        init_assembler(file_name, &assembler, parser.AC);
        run_assembler(&assembler, parser.queues);
    }
    free(source);
}
