#include "Token.h"
#include <stdio.h>
#include "stdlib.h"

void init_tokenlist(Tokenlist *tokenlist, size_t initial) {
    tokenlist->count = 0;
    tokenlist->capacity = initial;
    tokenlist->tokens = malloc(sizeof(Token) * initial);
}

void insert(Tokenlist *tokenlist, const Token* token) {
    if (tokenlist->capacity == tokenlist->count) {
        tokenlist->capacity *= 2;
        Token* temp = realloc(tokenlist->tokens, sizeof(Token) * tokenlist->capacity);
        if (temp == NULL) {
            fprintf(stderr, "Cannot allocate memory");
            exit(EXIT_FAILURE);
        }
        tokenlist->tokens = temp;
    }

    tokenlist->tokens[tokenlist->count++] = *token;
}

char* type_convert(const enum TokenType type) {
    switch (type) {
        case LEFT_PEREN:
            break;
        case RIGHT_PEREN:
            break;
        case LEFT_BRACE:
            break;
        case RIGHT_BRACE:
            break;
        case COMMA:
            break;
        case MINUS:
            break;
        case PLUS:
            break;
        case SLASH:
            break;
        case STAR:
            break;
        case LESS_THAN:
            break;
        case MORE_THAN:
            break;
        case MORE_EQUAL:
            break;
        case LESS_EQUAL:
            break;
        case AND:
            break;
        case ELSE:
            break;
        case IF:
            break;
        case OR:
            break;
        case ELSE_IF:
            break;
        case BANG:
            break;
        case BANG_EQUAL:
            break;
        case EQUAL_EQUAL:
            break;
        case FOR:
            break;
        case WHILE:
            break;
        case SUPER:
            break;
        case EQUAL:
            break;
        case VAR:
            break;
        case RETURN:
            break;
        case THIS:
            break;
        case INTEGER:
            return "integer";
            break;
        case CHARACTER:
            return "char";
            break;
        case BOOLEAN:
            return "bool";
        case FLOAT:
            return "float";
        case DOUBLE:
            return "double";
        case STRING:
            return "string";
        case EOF:
            break;
    }

}


void token_printer(const Tokenlist* list) {
   for (size_t i = 0; i < list->count; i++) {
      printf("%s, %s\n", list->tokens[i].lexeme, type_convert(list->tokens[i].type));
   }
}