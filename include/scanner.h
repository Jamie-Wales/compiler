#ifndef INCLUDE_SCANNER_H
#define INCLUDE_SCANNER_H

#include "Token.h"
#include <stdio.h>

char *generate_buff(char *path);

typedef struct Scanner {
    char *buffer;
    int index;
    int line;
    char *current_token;
    size_t size;
} Scanner;


void init_scanner(const char *path, Scanner *scanner);

char scanner_current(const Scanner *scanner);
char scanner_peek(const Scanner *scanner);
void scanner_walk(Scanner *scanner, Tokenlist *list);

#endif // INCLUDE_INCLUDE_SCANNER_H_
