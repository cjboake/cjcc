#ifndef GEN_H
#define GEN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

void assembly_header();
void emit_intexpr(Ast *ast);
void emit_op(Ast *ast);
void alloc_funct_args(Ast **a);
void alloc_var(Ast *var);
void emit_func(Ast *ast);
void compile(List *block);
void emit_expr(Ast *ast, int i);

#endif /* LEX.H */
