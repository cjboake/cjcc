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

const char symbols[] = { '"', ',', '{', '}', '.', '#', '<', '>','+', '-', '(', ')', ';' };

const char lines[] = { '\n', '\t' };

#define BUFLEN 256
#define SYM 14

enum {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE
};

enum {
    NUM,
    SYMB,
    CH
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

// check for whitespace
int check_white(char c)
{
    int r = 0;
    if(r == ' ')
        r = 1;
    return r;
}

// pass input and create token
void token_init(char input[])
{
    //char *p = (char*)&input;
    //Token tok = { .type = ID, .val = input};
    //return tok;
}

// TODO make string
char *make_string(char c)
{
    int i = 0;
    char tmp[32];
    char *str;
    
    tmp[i] = c;
    i++;
    str = &tmp[0];
    return str;
}


// is string in keyword table
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

// TODO is an operator

// is symbol a reserved symbol
int is_symbol(char c)
{
    int r = 0; 
    int i = 0;
    for(i = 0; i < SYM; i++) {
       if(c == symbols[i])
           r = 1;
    }
    return r;
}

// analyze symbols as they are passed through
int read_input(char c)
{
    char *buf;
    int r = 0;
    int t = 0; 
    if(c == '\n' || c == '\t')
        printf("This is either a newline or tab.\n");
    if(is_symbol(c))
        r = SYMB;
    if(isalpha(c)) 
        r = CH;
    if(isdigit(c))
        r = NUM;
    return r;
}

int fpeek(FILE *stream)
{
    int c;
    c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

void read_string(FILE *fp, char d, char buf[])
{
    int i = 0;
    buf[i] = d;
    i++;
    for(;;) {
        char c = getc(fp); 
        if(c == EOF)
            break;
        if(c == ' ')
            break;
        buf[i++] = c;
    }
    buf[i] = '\0';
}

int lex(char *input)
{
    char c;
    char buffer[BUFLEN];
    FILE *fp;
    fp = fopen(input, "r");
    char *y;

    for(;;) {
        c = fgetc(fp);
        char d = fpeek(fp);
        int r = read_input(c);
        if(r == CH) {
            read_string(fp, c, buffer);
            printf("Buffer print: %s\n", buffer);
        }
        if(c == EOF)
            break;
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
