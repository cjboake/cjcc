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
void compile(List *block);
void emit_func_header(Ast *ast);
void emit_expr(Ast *ast, int i);
void emit_intexp(Ast *a);
void emit_function(Ast *ast);

#endif /* LEX.H */
