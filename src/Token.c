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