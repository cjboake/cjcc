#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"
#include "parse.h"
#include "util.h"

#define BUFLEN 256

Token *read_ident(FILE *fp, char d);

void unget_token(FILE *fp, Token *tok)
{
    ungetc(tok->punct, fp);    
}

Token *make_int_tok(int n)
{
    Token *r = malloc(sizeof(Token));
    r->type = TTYPE_INT; 
    r->ival = n;
    return r;
}

Token *make_char_tok(int ch)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_CHAR;
    tok->c = ch;
    return tok;
}

Token *read_string(FILE *fp, char ch)
{
    Token *str_tok = read_ident(fp, ch);
    return str_tok; 
}

Token *make_string_tok(char *string)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_IDENT;
    tok->sval = string; 
    return tok;
}

void skip_space(FILE *fp)
{
    int c;
    while((c = fgetc(fp)) != EOF) {
        if(isspace(c)){
            continue;
        }else{ 
            ungetc(c, fp);
            break;
        }
    }
}

Token *read_ident(FILE *fp, char d)
{
    char *buf = malloc(BUFLEN);
    buf[0] = d;
    int i = 1;
    for(;;) {
        char c = getc(fp); 
        if(!isalnum(c) || c == '"' || c == ')'){
            ungetc(c, fp);
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return make_string_tok(buf);
}

Token *read_num(FILE *fp, int n)
{
    for(;;){
        int c = getc(fp);    
        if(!isdigit(c) || c == ';'){
            if(c == ';'){
                ungetc(c, fp);
                Token *tok = make_int_tok(n);
                return tok;
            }
            Token *tok = make_int_tok(n);
            return tok;
        } // else if c == something else, untermed? 
        n = n * 10 + (c - '0');
    }
}

Token *read_char(FILE *fp, int ch)
{
    int c = fgetc(fp);
    return make_char_tok(c);
}

Token *make_punc_tok(int c)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_PUNCT;
    tok->punct = c;
    return tok;
}

Token *debugger()
{
    printf("In the EOF Function\n");
    return NULL; 
}

Token *read_token(FILE *fp)
{
    skip_space(fp);
    int c = fgetc(fp);
    //printf("C: %c\n", c); 
    if(c == EOF){
        printf("GOT AN EOF\n");
        return NULL;//error("Unexpected EOF\n");
    }
    switch(c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return read_num(fp, c - '0');
        case '"':
            return read_string(fp, c);
        case '\'':
            return read_char(fp, c);
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B':
        case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W':
        case 'X': case 'Y': case 'Z': case '_': case '*': case '&':
            return read_ident(fp, c);
        case '/': case '=': case '#': case '+': case '-': case '(': case ')':
        case ',': case ';': case '{': case '}':
            return make_punc_tok(c);
        case EOF:
            return debugger();
        default:
            printf("Unexpected Character %d", c);
    }
    return NULL;
}

void print_ast(Ast *ast)
{
    printf("-> Print AST : %d\n", ast->type);
    switch (ast->type) {
        case AST_PLUS:
            printf("(+ ");
            goto print_op;
        case AST_MINUS:
            printf("(- ");
        print_op:
            print_ast(ast->left);
            printf(" ");
            print_ast(ast->right);
            printf(")");
            break;
        case AST_FUNC:
            printf("%s(", ast->fname);
            printf(")");
            //print_ast(ast->body);
            break;
        case AST_INT:
            printf("%d", ast->ival);
            break;
        case AST_STR:
            printf("%s", ast->sval);
            break;
        case AST_VAR:
            printf("%s", ast->name);
            print_ast(ast->value);
        default:
            printf("( %c", ast->type);
            print_ast(ast->left);
            printf(" ");
            print_ast(ast->right);
            printf(" )");
    }
}
