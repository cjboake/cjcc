#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT_PAREN = '(';
#define RIGHT_PAREN = ')'; 
#define AST = '*';
#define LEFT_BR = '[';
#define RIGHT_BR = ']';
#define SEMI_COL = ';';
#define LEFT_BRAC = '{';
#define RIGHT_BRAC = '}';

#define RETURN = "return";
#define INT = "int"; 
#define MAIN = "main";
#define ARGV = "argv";
#define ARGC = "argc";
#define CHAR = "char";

#define ONE = 1

#define BUFLEN = 256

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

int lex(char *input)
{
    char *buffer;
    long length;
    char *token[132];
    FILE *fp;
    int c, i = 0;
    char *delim, file_string[132];

    fp = fopen(input, "r");
    delim = "\0";
       
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

    if (buffer) {
        token[0] = strtok( buffer, delim );
        while(token[i] != NULL && i < 132) {
            i++;
            token[i] = strtok( buffer, delim );
        }
    }

    printf("This is token[1], %s\n", token[3]);

    return 0;
}

int main(int argc, char *argv[])
{
    char *input;
    
    if(argv[1] != NULL)
        input = argv[1];
        lex(input); 
//   parse();    
    
    return 0;
}

int parse()
{





    return 0;
}
