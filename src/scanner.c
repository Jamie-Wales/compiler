#include "scanner.h"
#include "utils.h"

#include <_types.h>
#include <ctype.h>
#include <stdbool.h>
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
    rewind(file);
    char *buff = malloc(fs + 1);
    if (buff == NULL) {
        fclose(file);
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
    if (scanner->index < scanner->size) {
        const int position = strlen(scanner->current_token);
        scanner->current_token[position] = scanner->buffer[scanner->index];
        scanner->index++;
        scanner->current_token[position + 1] = '\0';
    }
}

static void process_number(Scanner *scanner, Tokenlist *tok) {
    bool fpn = false;
    bool doub = true;

    while (scanner_peek(scanner) != '\0' && isdigit(scanner_current(scanner))) {
        if (scanner_peek(scanner) == '.') {
            scanner_advance(scanner);
            if (isdigit(scanner_peek(scanner))) {
                fpn = true;
                scanner_advance(scanner);
            } else {
                fprintf(stderr, "float double cannot end with . %i, %zu", scanner->line, tok->count);
            }
        }
        scanner_advance(scanner);
    }

    if (scanner_current(scanner) == 'f') {
        scanner_advance(scanner);
        doub = false;
    }

    Token *token = malloc(sizeof(Token));
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
    memset(scanner->current_token, 0, strlen(scanner->current_token) + 1);
}

char scanner_peek(const Scanner *scanner) {
    if (scanner->index == scanner->size) {
        return '\0';
    }
    return scanner->buffer[scanner->index + 1];
}

char scanner_current(const Scanner *scanner) {
    if (scanner->index == scanner->size) {
        return '\0';
    }
    return scanner->buffer[scanner->index];
}

static int cmp(const Scanner *scanner, const int num, const char *comparision) {
    char comparator[num];
    memcpy(comparator, scanner->buffer + scanner->index, num);
    return strncmp(comparator, comparision, num) == 0 && !isalpha(scanner->buffer[scanner->index + num]);
}

static int process_keywords(Scanner *scanner, Tokenlist *list) {
    const int count = list->count;
    switch (scanner_current(scanner)) {
        case 'f': {
            if (cmp(scanner, 3, "for")) {
                insert_generated_token(FOR, list);
                scanner->index += 3;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 5, "false")) {
                insert_generated_token(FALSE, list);
                scanner->index += 5;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 5, "float")) {
                insert_generated_token(FLOAT_L, list);
                scanner->index += 5;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 8, "function")) {
                insert_generated_token(FUNCTION, list);
                scanner->index += 8;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'w': {
            if (cmp(scanner, 5, "while")) {
                insert_generated_token(WHILE, list);
                scanner->index += 5;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'e': {
            if (cmp(scanner, 7, "else if")) {
                insert_generated_token(ELSE_IF, list);
                scanner->index += 7;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 4, "else")) {
                insert_generated_token(ELSE, list);
                scanner->index += 4;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'i': {
            if (cmp(scanner, 2, "if")) {
                insert_generated_token(IF, list);
                scanner->index += 2;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 3, "int")) {
                insert_generated_token(INTEGER_L, list);
                scanner->index += 3;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 't': {
            if (cmp(scanner, 4, "true")) {
                insert_generated_token(TRUE, list);
                scanner->index += 4;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 4, "this")) {
                insert_generated_token(THIS, list);
                scanner->index += 4;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'c': {
            if (cmp(scanner, 5, "class")) {
                insert_generated_token(CLASS, list);
                scanner->index += 5;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 4, "char")) {
                insert_generated_token(CHARACTER_L, list);
                scanner->index += 4;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            } else if (cmp(scanner, 5, "const")) {
                insert_generated_token(CONST, list);
                scanner->index += 5;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'l':
            if (cmp(scanner, 3, "let")) {
                insert_generated_token(LET, list);
                scanner->index += 3;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
            break;
        case 'r':
            if (cmp(scanner, 6, "return")) {
                insert_generated_token(RETURN, list);
                scanner->index += 6;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
            break;
        case 'b': {
            if (cmp(scanner, 4, "bool")) {
                insert_generated_token(BOOLEAN_L, list);
                scanner->index += 4;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 'd': {
            if (cmp(scanner, 6, "double")) {
                insert_generated_token(DOUBLE_L, list);
                scanner->index += 6;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
        case 's': {
            if (cmp(scanner, 6, "string")) {
                insert_generated_token(STRING_L, list);
                scanner->index += 6;
                memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
            }
        }
        break;
    }

    return list->count == count;
}


static void process_identifier(Scanner *scanner, Tokenlist *list) {
    while (scanner_peek(scanner) != '\0' && (isalpha(scanner_current(scanner)) || scanner_current(scanner) == '_')) {
        scanner_advance(scanner);
    }
    Token *token = malloc(sizeof(Token));
    token->type = IDENTIFIER;
    token->lexeme = strdup(scanner->current_token);
    insert(list, token);
    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
}

void scanner_walk(Scanner *scanner, Tokenlist *list) {
    while (scanner_peek(scanner) != '\0') {
        const char current = scanner_current(scanner);
        if (isdigit(current)) {
            process_number(scanner, list);
        } else if (isalpha(current)) {
            if (process_keywords(scanner, list) != 0) {
                process_identifier(scanner, list);
            }
        } else if (ispunct(current)) {
            switch (current) {
                case '(': {
                    insert_generated_token(LEFT_PEREN, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
                case ')': {
                    insert_generated_token(RIGHT_PEREN, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
                case '{': {
                    insert_generated_token(LEFT_BRACE, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
                case '}': {
                    insert_generated_token(RIGHT_BRACE, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
                case ';': {
                    insert_generated_token(SEMI_COLON, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
                case ',': {
                    insert_generated_token(COMMA, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;

                case '-': {
                    const enum TokenType t = scanner_peek(scanner) == '=' ?  MINUS_EQUAL : MINUS;
                    insert_generated_token(t, list);
                    scanner->index += 1;
                    memset(scanner->current_token, 0, strlen(scanner->current_token + 1));
                }
                break;
            }
        } else {
            scanner->index++;
        }
    }
}

void init_scanner(const char *path, Scanner *scanner) {
    scanner->buffer = string_buffer(path);
    scanner->index = 0;
    scanner->line = 0;
    scanner->size = strlen(scanner->buffer) + 1;
    scanner->current_token = malloc(sizeof(char) * strlen(scanner->buffer) + 1);
    if (scanner->current_token == NULL) {
        errorf("cannot allocate memory");
    }
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
