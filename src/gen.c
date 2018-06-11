#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

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

void emit_op(Ast *ast)
{
    char *op;
    if(ast->type == '+')
        op = "add";
    if(ast->type == '-')
        op = "sub";
    emit_intexpr(ast->left);
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
    printf("alloc_var\n");
    if(var->type == AST_INT)
        printf("mov     dword ptr [rbp - 4], %d \n", var->ival);

    if(var->type == '+' || var->type == '-') 
        emit_op(var);
}

void emit_func(Ast *ast)
{
    printf(".globl  _%s\n", ast->fname);
    printf(".p2align    4, 0x90\n\n");
    printf("_%s:\n", ast->fname);
    printf("\tpush  rbp\n\t");
    printf("mov   rbp, rsp\n\t");
    
    // handle function arguments
    if(ast->nargs > 0)
        alloc_funct_args(ast->args);
    
    // read function contents, var or declaration
    // will need to implement a loop over the body Ast**
    //if(ast->body->type == AST_VAR)
      //  alloc_var(ast->var); 
   
}

void compile(Ast *ast)
{
    assembly_header();
    //if(ast->type == AST_FUNC) {     
    //    emit_func(ast->body); 
    //}
    if(ast->type == AST_VAR) {     
    //    printf("In the new AST_VAR block\n"); 
//        emit_intexpr(ast); 
    }
    if(ast->type == AST_INT) {     
    //    emit_intexpr(ast); 
    }
    if(ast->type == AST_PLUS || ast->type == AST_MINUS) {
   //     emit_op(ast); 
    }
}
