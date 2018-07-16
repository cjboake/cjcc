#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"
#include "util.h"
#include "list.h"

#define EXPR_LEN 100

void emit_func(Ast *ast);
void emit_expr(Ast *ast);
void return_statement(Ast *ast);
void print_ret();

char *REGS[] = {"edi", "esi", "edx", "ecx", "r8", "r9"};

List *VARS = EMPTY_LIST;

void assembly_header()
{
    printf("\nASSEMBLY CODE\n\n");
    printf(".section    __TEXT,__text,regular,pure_instructions\n");
    printf(".macosx_version_min 10, 13\n");
    printf(".intel_syntax noprefix\n");
}

int is_var(Ast *ast)
{
    return ast->type == AST_VAR;
}

void emit_intexpr(Ast *ast)
{
    if(ast->type == AST_INT)
        printf("\tmov   rax, %d\n", ast->ival);
}

void emit_op(Ast *ast)
{
    printf("emit_op\n");
    char *op;
    if(ast->type == AST_PLUS) op = "add";
    if(ast->type == AST_MINUS) op = "sub";
    //emit_intexpr(ast->left);  for non-vars, handle later
   
    if(is_var(ast->left))
        printf("mov     eax, dword ptr [rbp - %d]\n\t", ast->left->vpos * 4);     
    if(is_var(ast->right)){
        printf("%s     eax, dword ptr [rbp - %d]\n\t", op, ast->right->vpos * 4);
    }
  //if(ast->right->type == AST_INT)
     // printf("\tmov rbx, %d\n\t", ast->right->ival);
    
  //printf("%s rax, rbx\n\t", op);
  //printf("mov rbi, 0\n\t");
}

void alloc_funct_args(Ast *a)
{
    for(int i = 1;i <= a->nargs;i++){
        printf("mov     dword ptr [rbp - %d], %s\n\t", i*4, REGS[i-1]);
    }
}

void print_ret()
{
    printf("pop     rbp\n\t");
    printf("ret\n\n");
}

void return_statement(Ast *ast)
{
    emit_expr(ast);
    print_ret();
}

void alloc_var(Ast *var)
{
    Tuple *variable = malloc(sizeof(Type));
    variable->name = var->name;
    if(var->type == AST_PLUS || var->type == AST_PLUS){
        printf("where we want to be\n");
        
        //emit_op(var);
    }
    if(var->pointer == 1){
        variable->pos = var->value->vpos * 4;
        printf("lea     rax, [rbp - %d]\n\t", var->value->ref_pos * 4);
        printf("mov     qword ptr [rbp - %d], rax\n\t", var->value->vpos * 4);
    }else if(var->value->type == AST_INT){
        variable->pos = var->vpos * 4;
        printf("mov     dword ptr [rbp - %d], %d\n\t", var->vpos * 4,  var->value->ival);
    }else if(var->type == '+' || var->type == '-'){
        emit_op(var);
    } else if(var->type == AST_DECL){
        alloc_var(var->value);
    }
    list_append(VARS, variable);
}

void mov_args(Ast **args)
{
    for(int u = 0; u < 7; u++){
        if(!args[u]) break;
        for (Iter *i = list_iter(VARS); !iter_end(i);) {
            Tuple *v = iter_next(i); 
            if(!strcmp(args[u]->name, v->name)){ 
                printf("mov     %s, dword ptr [rbp - %d]\n\t", REGS[u], v->pos);;
                break;
            }
        }
    }
}

void emit_expr(Ast *ast)
{
    if(ast->type == AST_FUNC) {     
        emit_func(ast); 
        alloc_funct_args(ast);
        for(int i = 0;i < EXPR_LEN; i++){
            if(ast->body[i]){
                emit_expr(ast->body[i]);
            }
            if(ast->body[i] == NULL) break;
        }
    }
    if(ast->type == AST_PLUS || ast->type == AST_MINUS){
        emit_op(ast);          
    }
    if(ast->type == AST_DECL){
        alloc_var(ast);
    }
    if(ast->type == AST_REF){ 
        mov_args(ast->args); 
        printf("call \t_%s\n\t", ast->fname);
    }
    if(ast->type == AST_VAR){ 
        printf("we have a var: %s\n\t", ast->name);
        //alloc_var(ast);
    }
    if(ast->type == AST_RET){
        return_statement(ast->ret_val);
    }
}

void emit_func(Ast *ast)
{
    printf(".globl  _%s\n", ast->fname);
    printf(".p2align    4, 0x90\n");
    printf("_%s:\n", ast->fname);
    printf("\tpush    rbp\n\t");
    printf("mov     rbp, rsp\n\t");
}

void compile(List *block)
{
    assembly_header();
    for (Iter *i = list_iter(block); !iter_end(i);)
        emit_expr(iter_next(i));
}


