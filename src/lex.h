#ifndef LEX_H
#define LEX_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

void unget_token(FILE *fp, Token *tok);
Token *make_char_tok(int ch);
Token *read_string(FILE *fp, char ch);
Token *make_string_tok(char *string);
void skip_space(FILE *fp);
Token *read_ident(FILE *fp, char d);
Token *read_num(FILE *fp, int n);
Ast *func_or_ident(FILE *fp, Token *tok);
Token *read_char(FILE *fp, int ch);
Token *make_punc_tok(int c);
Token *make_int_tok(int n);
Token *read_token(FILE *fp);
void print_ast(Ast *ast);

#endif /* LEX.H */
