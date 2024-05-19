#include "scanner.h"
#include "utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <_types.h>
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

static void scanner_advance(Scanner *scanner) {
    if (scanner->index != scanner->size) {
        const int position = strlen(scanner->current_token);
        scanner->current_token[position] = scanner->buffer[scanner->index];
        scanner->index++;
    }
}


static void process_number(Scanner *scanner, Tokenlist *tok) {
    bool fpn = false;
    bool doub = true;
    while (isdigit(scanner->buffer[scanner->index]) && scanner->index != scanner->size) {
        scanner_advance(scanner);
        if (scanner_peek(scanner) == '.') {
            scanner_advance(scanner);
            if (isdigit(scanner_peek(scanner))) {
                fpn = true;
                scanner_advance(scanner);
            } else {
                fprintf(stderr, "float double cannot end with . %i, %i", scanner->line, tok->count);
            }
        }
    }

    int position = strlen(scanner->current_token);
    if (scanner_current(scanner) == 'f') {
        scanner->current_token[position] = 'f';
        position++;
        doub = false;
    }

    scanner->current_token[position] = '\0';
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
        errorf("Cannot allocate token");
    }
    token->lexeme = malloc(sizeof(scanner->current_token));
    memcpy(token->lexeme, scanner->current_token, position);
    if (fpn) {
        if (doub) {
            token->type = DOUBLE;
        } else {
            token->type = FLOAT;
        }
    } else {
        token->type = INTEGER;
    }
    insert(tok, token);
    scanner->current_token = malloc(sizeof(char *) * strlen(scanner->buffer) - scanner->index);
}


char scanner_peek(const Scanner *scanner) {
    if (scanner->index == scanner->size) {
        return -1;
    }
    return scanner->buffer[scanner->index + 1];
}

char scanner_current(const Scanner *scanner) {
    if (scanner->index == scanner->size) {
        return -1;
    }
    return scanner->buffer[scanner->index];
}

void scanner_walk(Scanner *scanner, Tokenlist *list) {
    while (scanner_current(scanner)) {
        if (isdigit(scanner_current(scanner))) {
            process_number(scanner, list);
        } else {
            scanner->index++;
        }
    }
}

void init_scanner(const char *path, Scanner *scanner) {
    scanner->buffer = string_buffer(path);
    scanner->index = 0;
    scanner->line = 0;
    scanner->current_token = malloc(sizeof(scanner->buffer));
    scanner->size = strlen(scanner->buffer) + 1;
}

void freebuffer(char *buffer) {
    if (buffer != NULL)
        free(buffer);
}

void free_scanner(Scanner *scanner) {
    freebuffer(scanner->buffer);
    scanner->buffer = NULL;
    freebuffer(scanner->current_token);
    free(scanner);
}
