#include "token.h"
#include <scanner.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: %s <inputFile>\n", argv[0]);
        return 1;
    }
    Scanner* scanner = malloc(sizeof(Scanner));
    Tokenlist* list = malloc(sizeof(Tokenlist));
    init_scanner(argv[1], scanner);
    init_tokenlist(list, 1);
    scanner_walk(scanner, list);
    token_printer(list);
    return 0;
}
