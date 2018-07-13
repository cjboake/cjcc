#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"
#include "lex.h"

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
    printf("fpeek: %c\n", c);
    ungetc(c, stream);
    return c;
}

void error(char *input)
{
    fprintf(stderr, "%s\n", input);
    exit(0);
}

void ensure_ptr(Ast *node)
{
    if(node->name[0] != '&')
        error("Pointer value does not reference variable memory location.");
    node->name++;
}

void expect(FILE * fp, int c)
{
    Token *tok = read_token(fp);
    if(tok->punct != c){
        printf("Token value: %d", tok->punct);
        fprintf(stderr, "Error, did not get expected token '%c'.\n", c);
        exit(0);
    }
}

int check_for(char c, FILE *fp)
{
    Token *tok = read_token(fp);
    if(tok->punct == c){
        return 1; 
    } else {
        unget_token(fp, tok);
        return 0;
    }
}

void node_check(Ast *node, int type)
{
    if(node->type != type)
        error("An unexpected node type was received\n");
}

void p(char *input)
{
   printf("%s\n", input); 
}
