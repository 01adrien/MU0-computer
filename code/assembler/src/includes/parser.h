#ifndef _PARSER_H_
#define _PARSER_H_

#include "common.h"
#include "lexer.h"
#include "scanner.h"

typedef char* parse_error_t;

typedef struct node node_t;

struct node {
    token_t token;
    node_t* next;
};

typedef struct token_queue {
    node_t* head;
} token_queue_t;

typedef struct parser {
    lexer_t* lexer;
    scanner_t* scanner;
    bool is_error;
    token_queue_t** queues;
    uint32_t AC;
} parser_t;

void
init_parser(lexer_t* lexer, scanner_t* scanner, parser_t* parser);

parse_error_t
run_parser(parser_t* parser);

void
print_queue(token_queue_t* queue);

void
enqueue_token(token_queue_t* queue, token_t token);

node_t*
create_node(token_t token);

node_t*
peek_queue(token_queue_t* queue);

void
free_queue(token_queue_t* queue);

token_t
get_token(node_t* node);

node_t*
dequeue_token(token_queue_t* queue);

#endif
