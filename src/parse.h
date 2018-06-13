#ifndef PARSE_H
#define PARSE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

Ast *make_ast_operator(int type);
Ast *ast_string(char buffer[]);
Ast *make_var(char *name);
Ast *make_ast_var(Ast *varr, Ast *val);
Ast *make_ast_func(char *name, int n, Ast **a);
Ast *make_ast_int(int val);
Ast *make_ast_node(Ast *l, Ast *r, int op);
Ast *read_func_args(FILE *fp, char *buf);
Ast *make_var(char *name);
Ast *func_or_ident(FILE *fp, Token *tok);
//Ast *find_var(char *name);
#endif /* PARSE_H */
