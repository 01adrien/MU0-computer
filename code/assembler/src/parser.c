#include "includes/parser.h"
#include "includes/assembler.h"
#include "includes/utils.h"
#include <stdarg.h>

parse_error_t
parse_all(parser_t* parser);

bool
match_token(parser_t* parser, token_type_t type);

void
error_parser(const char* msg, ...);

void
init_parser(lexer_t* lexer, scanner_t* scanner, parser_t* parser) {
    parser->lexer = lexer;
    parser->scanner = scanner;
    parser->is_error = false;
    parser->AC = INSTRUCTION;
    parser->queues = calloc(3, sizeof(token_queue_t*));
    parser->queues[0] = calloc(1, sizeof(token_queue_t));
    parser->queues[1] = calloc(1, sizeof(token_queue_t));
    parser->queues[2] = calloc(1, sizeof(token_queue_t));
}

parse_error_t
run_parser(parser_t* parser) {
    lexer_t* lexer = parser->lexer;
    scanner_t* scanner = parser->scanner;
    advance_lexer(lexer, scanner);
    while (lexer->next.type != TOKEN_EOF) {
        advance_lexer(lexer, scanner);
        parse_error_t err = parse_all(parser);
        if (parser->is_error) {
            free_queue(parser->queues[0]);
            free_queue(parser->queues[1]);
            free_queue(parser->queues[2]);
            free(parser->queues);
            return err;
        }
    }
}

parse_error_t
parse_all(parser_t* parser) {
    lexer_t* lexer = parser->lexer;
    token_t* token = &lexer->current;
    scanner_t* scanner = parser->scanner;
    switch (token->type) {
        case TOKEN_DATA:
            if (!match_token(parser, TOKEN_IDENTIFIER))
                return (parse_error_t) "expecting identifier";
            enqueue_token(parser->queues[1], *token);
            if (!match_token(parser, TOKEN_NUMBER))
                return (parse_error_t) "expecting value after identifier";
            enqueue_token(parser->queues[1], *token);
            if (!match_token(parser, TOKEN_NEWLINE))
                return (parse_error_t) "unexpected symbol";
            break;
        case TOKEN_LDA:
        case TOKEN_STR:
        case TOKEN_ADD:
        case TOKEN_SUB:
        case TOKEN_JMP:
        case TOKEN_JLZ:
        case TOKEN_JEZ:
        case TOKEN_JGZ:
        case TOKEN_HLT:
        case TOKEN_NOT:
        case TOKEN_OR:
        case TOKEN_LDI:
        case TOKEN_LEA:
        case TOKEN_STA:
        case TOKEN_AND:
        case TOKEN_NEG:
            enqueue_token(parser->queues[0], *token);
            advance_lexer(lexer, scanner);
            enqueue_token(parser->queues[0], *token);
            parser->AC++;
            break;
        case TOKEN_LABEL:
            token->addr = parser->AC;
            enqueue_token(parser->queues[2], *token);
            break;
        case TOKEN_NEWLINE:
            break;
        default:
            printf("Unknow token : ");
            PRINT_TOKEN(*token);
            break;
    }
}

bool
match_token(parser_t* parser, token_type_t type) {
    if (parser->lexer->next.type == type) {
        advance_lexer(parser->lexer, parser->scanner);
        return true;
    }
    parser->is_error = true;
    return false;
}

void
error_parser(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    va_end(args);
    fprintf(stderr, "Error: ");
    fprintf(stderr, msg, args);
}

// QUEUE FUNCTION

void
print_queue(token_queue_t* queue) {
    node_t* temp = queue->head;
    while (temp) {
        PRINT_TOKEN(temp->token);
        temp = temp->next;
    }
}

void
enqueue_token(token_queue_t* queue, token_t token) {
    node_t* new = create_node(token);
    node_t* temp = queue->head;
    if (!temp) {
        queue->head = new;
        return;
    }
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = new;
}

node_t*
create_node(token_t token) {
    node_t* node = calloc(1, sizeof(node_t));
    node->token = token;
    node->next = NULL;
    return node;
}

node_t*
peek_queue(token_queue_t* queue) {
    return queue->head;
}

void
free_queue(token_queue_t* queue) {
    node_t* temp = queue->head;
    while (temp) {
        node_t* del = temp;
        temp = temp->next;
        free(del);
    }
}

token_t
get_token(node_t* node) {
    token_t token = node->token;
    free(node);
    return token;
}

node_t*
dequeue_token(token_queue_t* queue) {
    if (!queue->head) {
        return NULL;
    }
    node_t* res = queue->head;
    queue->head = res->next;
    return res;
}
