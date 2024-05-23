#include "token.h"
#include "stdlib.h"
#include <stdio.h>
#include <utils.h>

void init_tokenlist(Tokenlist *tokenlist, const size_t initial) {
    tokenlist->count = 0;
    tokenlist->capacity = initial;
    tokenlist->tokens = malloc(sizeof(Token) * initial);
}

void insert(Tokenlist *tokenlist, const Token *token) {
    if (tokenlist->capacity == tokenlist->count) {
        tokenlist->capacity *= 2;
        Token *temp = realloc(tokenlist->tokens, sizeof(Token) * tokenlist->capacity);
        if (temp == NULL) {
            errorf("Cannot allocate memory %s");
        }
        tokenlist->tokens = temp;
    }

    tokenlist->tokens[tokenlist->count++] = *token;
}

void insert_generated_token(const enum TokenType type, Tokenlist *list) {
    const Token *token = generate_token(type);
    insert(list, token);
}

char *type_convert(const enum TokenType type) {
    switch (type) {
        case IDENTIFIER:
            return "identifier";
        case LEFT_PEREN:
            return "(";
        case RIGHT_PEREN:
            return ")";
        case LEFT_BRACE:
            return "{";
        case RIGHT_BRACE:
            return "}";
        case COMMA:
            return ",";
        case MINUS:
            return "-";
        case PLUS:
            return "+";
        case SLASH:
            return "/";
        case STAR:
            return "*";
        case LESS_THAN:
            return "<";
        case MORE_THAN:
            return ">";
        case MORE_EQUAL:
            return ">=";
        case LESS_EQUAL:
            return "<=";
        case AND:
            return "&&";
        case ELSE:
            return "else";
        case IF:
            return "if";
        case OR:
            return "||";
        case ELSE_IF:
            return "else if";
        case BANG:
            return "!";
        case BANG_EQUAL:
            return "!=";
        case EQUAL_EQUAL:
            return "==";
        case FOR:
            return "for";
        case WHILE:
            return "while";
        case SUPER:
            return "super";
        case EQUAL:
            return "=";
        case LET:
            return "let";
        case CONST:
            return "const";
        case CLASS:
            return "class";
        case RETURN:
            return "return";
        case THIS:
            return "this";
        case INTEGER:
            return "integer";
        case INTEGER_L:
            return "int_t";
        case CHARACTER:
            return "char";
        case CHARACTER_L:
            return "char_l";
        case BOOLEAN_L:
            return "bool_l";
        case FLOAT:
            return "float";
        case FLOAT_L:
            return "float_l";
        case DOUBLE:
            return "double";
        case DOUBLE_L:
            return "double_t";
        case STRING:
            return "string";
        case STRING_L:
            return "string_t";
        case TRUE:
            return "true";
        case FALSE:
            return "false";
        case FUNCTION:
            return "function";
        default:
            return "";
    }
}

Token *generate_token(const enum TokenType type) {
    Token *output = malloc(sizeof(Token));
    if (output == NULL)
        return NULL;
    output->lexeme = type_convert(type);
    output->type = type;
    return output;
}

void token_printer(const Tokenlist *list) {
    for (size_t i = 0; i < list->count; i++) {
        printf("%s, %s\n", list->tokens[i].lexeme, type_convert(list->tokens[i].type));
    }
}

void free_token_list(Tokenlist *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(&list->tokens[i]);
    }
    free(list);
    list = NULL;
}
