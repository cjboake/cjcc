#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LEFT_PAREN '('
#define RIGHT_PAREN ')' 
#define AST '*'
#define LEFT_BR '['
#define RIGHT_BR ']'
#define SEMI_COL ';'
#define LEFT_BRAC '{'
#define RIGHT_BRAC '}'

#define RETURN "return"
#define INT "int"
#define MAIN "main"
#define ARGV "argv"
#define ARGC "argc"
#define CHAR "char"

const char *keywords[] = { RETURN, INT, MAIN, ARGV, ARGC, CHAR };

const char symbols[] = { '+', '-', '(', ')', ';', '{', '}' };

#define BUFLEN 256

enum {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE
};

enum {
    NUM,
    ID,
    WORD
};

enum {
    AST_PLUS,
    AST_MINUS,
    AST_INT,
    AST_STR,
};

typedef struct Ast {
  int type;
  union {
    int ival;
    char *sval;
    struct {
      struct Ast *left;
      struct Ast *right;
    };
  };
} Ast;

typedef struct Token {
    int type;
    char *val;
} Token;

Token token_init(char input[])
{
    char *p = (char*)&input;
    Token tok = { .type = ID, .val = input};
    return tok;
}

int is_keyword(char *word)
{
    int r = 0; 
    int i = 0;
    for(i = 0; i < 7; i++) {
       if(strcmp(word, keywords[i]))
           r = 1;
    }
    return r;
}

int is_symbol(char c)
{
    int r = 0; 
    int i = 0;
    for(i = 0; i < 8; i++) {
       if(c == symbols[i])
           r = 1;
    }
    return r;
}

int read_symbol(char c)
{
    int r = 0;
    if(is_symbol(c))
        r = 1;

    return r;
}

int read_keyword(char *c)
{
    if(c) {
    
    }    

    return 1;
}

int lex(char *input)
{
    char c;
    FILE *fp;
    fp = fopen(input, "r");
    if(fp == NULL) {
        printf("Error, file is null.\n");
        exit(1);
    }
    while((c = fgetc(fp)) != EOF) {
        fputc(c, stdout);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char *input;
    if(argv[1] != NULL) {
        input = argv[1];
        lex(input); 
    } else {
        input = "Please give an input\n";
        printf("%s", input);
    } 
    return 0;
}
