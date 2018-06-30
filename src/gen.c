#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

#define EXPR_LEN 100

void assembly_header()
{
    printf("\nASSEMBLY CODE\n\n");
    printf(".section    __TEXT,__text,regular,pure_instructions\n");
    printf(".macosx_version_min 10, 13\n");
    printf(".intel_syntax noprefix\n");
}

void emit_intexpr(Ast *ast)
{
    if(ast->type == AST_INT)
        printf("\tmov rax, %d\n", ast->ival);
}

// original emit_op, may not last
void emit_op(Ast *ast)
{
    char *op;
    if(ast->type == '+')
        op = "add";
    if(ast->type == '-')
        op = "sub";
    emit_intexpr(ast->left);
    printf("ast->right: %d\n", ast->left->ival);
    if(ast->right->type == AST_INT)
        printf("\tmov rbx, %d\n\t", ast->right->ival);
    printf("%s rax, rbx\n\t", op);
    printf("mov rbi, 0\n\t");
    printf("pop rbp\n\t");
    printf("ret\n");
}

void alloc_funct_args(Ast **a)
{
    printf("alloc_funct_args\n");
}

void alloc_var(Ast *var)
{
    if(var->type == AST_INT)
        printf("mov  dword ptr [rbp - %d], %d\n\t", var->vpos * 4,  var->ival);

    if(var->type == '+' || var->type == '-'){
        printf("we are in the operator block\n");
        emit_op(var);
    }
}

void emit_expr(Ast *ast)
{
    if(ast->type == AST_DECL)
        alloc_var(ast->value);
    if(ast->type == AST_VAR)
        printf("We made it guys!\n");
    if(ast->type == AST_RET)
        printf("We have a return!\n");
}

void emit_func(Ast *ast)
{
    printf(".globl  _%s\n", ast->fname);
    printf(".p2align    4, 0x90\n\n");
    printf("_%s:\n", ast->fname);
    printf("\tpush  rbp\n\t");
    printf("mov   rbp, rsp\n\t");
}

void compile(Ast *ast)
{
    assembly_header();
    if(ast->type == AST_FUNC) {     
      if(ast->nargs > 0)
        alloc_funct_args(ast->args);
        emit_func(ast); 
        for(int i = 0;i < EXPR_LEN; i++){
            if(ast->body[i])
                emit_expr(ast->body[i]);
            if(ast->body[i] == NULL) break;
        }
    }
}
