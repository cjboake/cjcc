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
    ungetc(c, stream);
    return c;
}

void error(char *input)
{
    fprintf(stderr, "%s", input);
    exit(0);
}

void expect(FILE * fp, int c)
{
    Token *tok = read_token(fp);
    if(tok->punct != c){
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


