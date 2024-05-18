#ifndef INCLUDE_SCANNER_H_
#define INCLUDE_SCANNER_H_

#include "Token.h"
#include <stdio.h>

char *generate_buff(char *path);

typedef struct Scanner {
    char *buffer;
    int index;
    int line;
    char *current_token;
} Scanner;


void init_scanner(char *path, Scanner *scanner);

void scanner_walk(Scanner *scanner, Tokenlist *list);

#endif // INCLUDE_INCLUDE_SCANNER_H_
