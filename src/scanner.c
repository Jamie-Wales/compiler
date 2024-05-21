#include "scanner.h"
#include "utils.h"

#include <_types.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>

static char* string_buffer(const char* path)
{
    test_file(path);
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        errorf("Cannot open file");
    }
    fseek(file, 0, SEEK_END);
    const long fs = ftell(file);
    rewind(file);
    char* buff = malloc(fs + 1);
    if (buff == NULL) {
        errorf("Cannot allocate buffer");
    }
    const size_t bread = fread(buff, 1, fs, file);
    if (bread < fs) {
        free(buff);
        fclose(file);
        errorf("Cannot read entire file");
    }
    buff[bread] = '\0';
    fclose(file);
    return buff;
}

static void scanner_advance(Scanner* scanner)
{
    if (scanner->index != scanner->size) {
        const int position = strlen(scanner->current_token);
        scanner->current_token[position] = scanner->buffer[scanner->index];
        scanner->index++;
        scanner->current_token[position + 1] = '\0';
    }
}

static void process_number(Scanner* scanner, Tokenlist* tok)
{
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
                fprintf(stderr, "float double cannot end with . %i, %zu", scanner->line, tok->count);
            }
        }
    }

    if (scanner_current(scanner) == 'f') {
        scanner_advance(scanner);
        doub = false;
    }

    Token* token = malloc(sizeof(Token));
    token->lexeme = strdup(scanner->current_token);
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
    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
}

char scanner_peek(const Scanner* scanner)
{
    if (scanner->index == scanner->size) {
        return -1;
    }
    return scanner->buffer[scanner->index + 1];
}

char scanner_current(const Scanner* scanner)
{
    if (scanner->index == scanner->size) {
        return -1;
    }
    return scanner->buffer[scanner->index];
}

static int cmp(Scanner* scanner, int num, char* comparision)
{
    char comparator[num];
    memcpy(comparator, scanner->buffer + scanner->index, num);
    return strncmp(comparator, comparision, num);
}

static void process_keywords(Scanner* scanner, Tokenlist* list)
{
    switch (scanner_current(scanner)) {
    case 'f': {
        if (cmp(scanner, 3, "for") == 0 && !isalpha(scanner->buffer[scanner->index + 3])) {
            Token* token = generate_token(FOR);
            scanner->index += 3;
            insert(list, token);
            memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
        } else if (cmp(scanner, 5, "false") == 0) {
            Token* token = generate_token(FALSE);
            scanner->index += 5;
            insert(list, token);
            memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
        } else {
            while (isalnum(scanner->buffer[scanner->index]) && scanner->index != scanner->size) {
                scanner_advance(scanner);
            }
            Token* token = malloc(sizeof(Token));
            token->type = IDENTIFIER;
            token->lexeme = strdup(scanner->current_token);
            insert(list, token);
            memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
        }
    } break;
    case 'w': {
        Token* token = generate_token(WHILE);
        scanner->index += 5;
        insert(list, token);
        memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
    } break;
    }
}

void scanner_walk(Scanner* scanner, Tokenlist* list)
{
    while (scanner_current(scanner)) {
        if (isdigit(scanner_current(scanner))) {
            process_number(scanner, list);
        } else if (isalpha(scanner_current(scanner))) {
            process_keywords(scanner, list);
        } else {
            scanner->index++;
        }
    }
}

void init_scanner(const char* path, Scanner* scanner)
{
    scanner->buffer = string_buffer(path);
    scanner->index = 0;
    scanner->line = 0;
    scanner->size = strlen(scanner->buffer) + 1;
    scanner->current_token = malloc(sizeof(char) * strlen(scanner->buffer) + 1);
    if (scanner->current_token == NULL) {
        errorf("cannot allocate memory");
    }
}

void freebuffer(char* buffer)
{
    if (buffer != NULL)
        free(buffer);
}

void free_scanner(Scanner* scanner)
{
    freebuffer(scanner->buffer);
    scanner->buffer = NULL;
    freebuffer(scanner->current_token);
    free(scanner);
}
