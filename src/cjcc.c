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

const char *symbols[] = { RETURN, INT, MAIN, ARGV, ARGC, CHAR };

#define BUFLEN 256

//} else {
  //          printf("Else block\n");
    //        Token t = { .type = ID, .val = tmp }; 
      //      arr[a] = t;
        //    a++;
        //    tmp[0] = 0; 
        //    t = EmptyToken;
       // }

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

int scan(char * buf)
{
    int lines = 0;
    int i;
    int s = 0;
    Token arr[10];
    int a = 0;
    char tmp[10];
    char peek = i + 1;

    for(i = 0; i < strlen(buf); i++) {
        if(buf[i] != ' ' && buf[i] != '\n') {
            tmp[s] = buf[i];
            s++;
        } 
        if(buf[i] == ' ') {
            char *p = (char*)&tmp;
            printf("This is the buffer: %s\n", p);
            
        }    
        
    }
    return 0;
}

int lex(char *input)
{
    Token toks[BUFLEN];
    char *buffer;
    long length;
    FILE *fp;
    int lines;
    int i = 0;
    
    fp = fopen(input, "r");
    
    if (fp) {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer) {
            fread (buffer, 1, length, fp);
        }
        fclose (fp);
    }
    
    int h = scan(buffer);
        //toks[i] = tok;
        i++;
     
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
