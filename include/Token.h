#ifndef INCLUDE_TOKEN_H_
#define INCLUDE_TOKEN_H_
#include <sys/_types/_size_t.h>


enum TokenType {
    /* ---- SYNTAX ---- */
    LEFT_PEREN,
    RIGHT_PEREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,

    /* ---- MATHMATICAL OPERATORS ---- */
    MINUS,
    PLUS,
    SLASH,
    STAR,
    LESS_THAN,
    MORE_THAN,
    MORE_EQUAL,
    LESS_EQUAL,

    /* ---- LOGICAL OPERATORS ---- */
    AND,
    ELSE,
    IF,
    OR,
    ELSE_IF,
    BANG,
    BANG_EQUAL,
    EQUAL_EQUAL,

    /* ---- KEYWORDS ---- */
    FOR,
    WHILE,
    SUPER,
    EQUAL,
    VAR,
    RETURN,
    THIS,


    /* ---- TYPES ----*/
    INTEGER,
    CHARACTER,
    BOOLEAN,
    FLOAT,
    DOUBLE,
    STRING,

    EOF,
};

typedef struct Token {
    char *lexeme;
    enum TokenType type;
} Token;

typedef struct Tokenlist {
    Token* tokens;
    size_t count;
    size_t capacity;
} Tokenlist;

void insert(Tokenlist *tokenlist, const Token *token);
void init_tokenlist(Tokenlist *tokenlist, size_t initial);

#endif