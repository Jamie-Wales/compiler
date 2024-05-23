#ifndef INCLUDE_TOKEN_H
#define INCLUDE_TOKEN_H
#include <sys/_types/_size_t.h>

enum TokenType {
    /* ---- SYNTAX ---- */
    LEFT_PEREN,
    RIGHT_PEREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMI_COLON,
    COMMA,

    /* ---- MATHMATICAL OPERATORS ---- */
    MINUS,
    MINUS_EQUAL,
    PLUS,
    PLUS_EQUAL,
    SLASH,
    SLASH_EQUAL,
    STAR,
    STAR_EQUAL,
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
    TRUE,
    FALSE,

    /* ---- KEYWORDS ---- */
    FOR,
    WHILE,
    CLASS,
    SUPER,
    EQUAL,
    LET,
    CONST,
    RETURN,
    THIS,
    FUNCTION,


    INTEGER_L,
    CHARACTER_L,
    BOOLEAN_L,
    FLOAT_L,
    DOUBLE_L,
    STRING_L,

    /* ---- TYPES R VALUE ----*/
    INTEGER,
    CHARACTER,
    FLOAT,
    DOUBLE,
    STRING,
    IDENTIFIER,

    EOF,
};

typedef struct Token {
    char* lexeme;
    enum TokenType type;
} Token;

typedef struct Tokenlist {
    Token* tokens;
    size_t count;
    size_t capacity;
} Tokenlist;

void insert(Tokenlist* tokenlist, const Token* token);
void init_tokenlist(Tokenlist* tokenlist, size_t initial);
void token_printer(const Tokenlist* list);
char* type_convert(enum TokenType type);
Token* generate_token(enum TokenType type);
void insert_generated_token(const enum TokenType type, Tokenlist* list);
void free_token_list(Tokenlist* list);
#endif
