#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RETURN "return"
#define INT "int"
#define MAIN "main"
#define ARGV "argv"
#define ARGC "argc"
#define CHAR "char"

#define BUFLEN 256
#define SYM 14

const char *keywords[] = { RETURN, INT, MAIN, ARGV, ARGC, CHAR };

const char symbols[] = { '"', ',', '{', '}', '.', '#', '<', '>','+', '-', '(', ')', ';' };

const char lines[] = { '\n', '\t' };

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

void check_file(FILE *p)
{
    if(p == NULL) {
        printf("The file failed to open.\n");
        exit(0);
    }
}

int fpeek(FILE *stream)
{
    int c;
    c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

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

void read_binop()
{


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

Ast *make_ast_int(int val)
{
    Ast *r = malloc(sizeof(Ast));
    r->type = AST_INT;
    r->ival = val;
    return r;
}

Ast *read_num(FILE *fp, int n)
{
    for(;;){
        int c = getc(fp);    
        if(!isdigit(c)){
            ungetc(c, fp);        
            Ast *t = make_ast_int(n);
            return t;
        } 
    n = n * 10 + (c - '0');
    }
}

Ast *scan(char *input)
{
    int c;
    char buffer[BUFLEN];
    FILE *fp;
    fp = fopen(input, "r");
    char *y;
    int n;
    
    fp = fopen(input, "r");
    check_file(fp);
    for(;;) {
        c = fgetc(fp);
        int r = read_input(c);
        if(c == EOF)
            break;
        if(r == CH) {
            read_string(fp, c, buffer);
            printf("Buffer print: %s\n", buffer);
        }
        if(r == NUM){
            n = c - '0';
            Ast *r = read_num(fp, n);     
            printf("Scan Ast->ival: %d\n", r->ival);        
        }    
        if(r == SYMB) {
            if(c == '+' || c == '-')
                printf("Operator\n");        
        }
    }
    return 0;
}

void run(char *argv[])
{
    int r = 0;
    char *input;
    if(argv[1] != NULL) {
        input = argv[1];
        Ast *ast = scan(input); 
    } else {
        input = "Please give an input\n";
        printf("%s", input);
    }
}

int main(int argc, char *argv[])
{
    run(argv);
    return 0;
}
