#include "scanner.h"
#include "utils.h"
#include <_types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>

static char *string_buffer(const char *path) {
    test_file(path);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        errorf("Cannot open file");
    }
    fseek(file, 0, SEEK_END);
    long fs = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buff = malloc(fs + 1);
    if (buff == NULL) {
        errorf("Cannot allocate buffer");
    }
    size_t bread = fread(buff, 1, fs, file);
    if (bread < fs) {
        free(buff);
        fclose(file);
        errorf("Cannot read entire file");
    }
    buff[bread] = '\0';
    fclose(file);
    return buff;
}

static void processNumber(Scanner *scanner, Tokenlist *tok) {
    int position = 0;
    while (isdigit(scanner->buffer[scanner->index])) {
        scanner->current_token[position] = scanner->buffer[scanner->index];
        scanner->index++;
        position++;
    }

    scanner->current_token[position] = '\0';
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
        errorf("Cannot allocate token");
    }
    token->lexeme = malloc(sizeof(scanner->current_token + 1));
    memcpy(token->lexeme, scanner->current_token, position);
    token->type = INTEGER;
    insert(tok, token);
    scanner->current_token = malloc(sizeof(char *) * strlen(scanner->buffer) - scanner->index);
}


void scanner_walk(Scanner *scanner, Tokenlist *tok) {
    while (scanner->index != strlen(scanner->buffer)) {
        if (isdigit(scanner->buffer[scanner->index])) {
            processNumber(scanner, tok);
        } else {
            scanner->index++;
        }
    }
}

void init_scanner(char *path, Scanner *scanner) {
    scanner->buffer = string_buffer(path);
    scanner->index = 0;
    scanner->line = 0;
    scanner->current_token = malloc(sizeof(scanner->buffer + 1));
}

void freebuffer(char *buffer) {
    free(buffer);
}

void freeScanner(Scanner **scan) {
    Scanner *scanner = *scan;
    freebuffer(scanner->buffer);
    freebuffer(scanner->current_token);
    free(scanner);
}
