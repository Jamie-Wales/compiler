#include "token.h"
#include "stdlib.h"
#include <stdio.h>

void init_tokenlist(Tokenlist* tokenlist, const size_t initial)
{
    tokenlist->count = 0;
    tokenlist->capacity = initial;
    tokenlist->tokens = malloc(sizeof(Token) * initial);
}

void insert(Tokenlist* tokenlist, const Token* token)
{
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

char* type_convert(const enum TokenType type)
{
    switch (type) {
    case IDENTIFIER:
        return "identifier";
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
        return "for";
    case WHILE:
        return "while";
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
    case CHARACTER:
        return "char";
    case BOOLEAN:
        return "bool";
    case FLOAT:
        return "float";
    case DOUBLE:
        return "double";
    case STRING:
        return "string";
    case TRUE:
        return "true";
    case FALSE:
        return "false";
    default:
        return "";
    }
}

Token* generate_token(const enum TokenType type)
{
    Token* output = malloc(sizeof(Token));
    if (output == NULL)
        return NULL;
    output->lexeme = type_convert(type);
    output->type = type;
    return output;
}

void token_printer(const Tokenlist* list)
{
    for (size_t i = 0; i < list->count; i++) {
        printf("%s, %s\n", list->tokens[i].lexeme, type_convert(list->tokens[i].type));
    }
}
