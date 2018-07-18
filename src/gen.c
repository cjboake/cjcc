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

List *VARS = EMPTY_LIST;
char *REGS[] = {"edi", "esi", "edx", "ecx", "r8", "r9"};

int is(Ast *a, int type)
{
    if(a->type == type)
        return 1;
    return 0;
}

void emit_intexpr(Ast *a)
{
    if(is(a, AST_INT)) 
        printf("\tmov   rax, %d\n", a->ival);
}

void emit_op(Ast *ast)
{
    char *op;
    if(ast->value->type == AST_PLUS) op = "add";
    if(ast->value->type == AST_MINUS) op = "sub";
  
    printf("mov     eax, dword ptr [rbp - %d]\n\t", ast->value->left->vpos * 4);     
    printf("%s     eax, dword ptr [rbp - %d]\n\t", op, ast->value->right->vpos * 4);
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
    //emit_expr(ast);
    //printf("mov     eax, dword ptr [rbp - %d]\n\t", ast->vpos*4);
    print_ret();
}

void emit_pointer(Ast *var)
{   
    Tuple *variable = malloc(sizeof(Type));
    variable->name = var->name;
    variable->pos = var->value->vpos * 4;
    list_append(VARS, variable);
    printf("lea     rax, [rbp - %d]\n\t", var->value->ref_pos * 4);
    printf("mov     qword ptr [rbp - %d], rax\n\t", var->value->vpos * 4);
}

void emit_int(Ast *ast)
{
    Tuple *variable = malloc(sizeof(Type));
    variable->name = ast->name;
    printf("mov     dword ptr [rbp - %d], %d\n\t", ast->vpos * 4,  ast->value->ival);
    list_append(VARS, variable);
}

void alloc_var(Ast *ast)
{
    switch(ast->value->type){
        case AST_PLUS: emit_op(ast);
            break;
        case AST_MINUS: emit_op(ast);
            break;
        case AST_INT: emit_int(ast);  
            break;
        case AST_PTR: emit_pointer(ast);
            break;
        default: printf("Unknown var type\n");
    }
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

void call_function(Ast *ast)
{
    mov_args(ast->args); 
    printf("call \t_%s\n\t", ast->fname);
}

void emit_func_header(Ast *ast)
{
    printf(".globl  _%s\n", ast->fname);
    printf(".p2align    4, 0x90\n");
    printf("_%s:\n", ast->fname);
    printf("\tpush    rbp\n\t");
    printf("mov     rbp, rsp\n\t");
}

void emit_function(Ast *ast)
{
    emit_func_header(ast); 
    alloc_funct_args(ast);
    for(int i = 0;i < EXPR_LEN; i++) {
        if(ast->body[i]) emit_expr(ast->body[i]);
        if(ast->body[i] == NULL) break;
    }
}

void emit_expr(Ast *ast)
{
    switch(ast->type){
        case AST_FUNC:    
            emit_function(ast); 
            break;
        case AST_PLUS:  
            emit_op(ast);
            break;
        case AST_MINUS:
            emit_op(ast);
            break;
        case AST_DECL:
            alloc_var(ast);
            break;
        case AST_REF: 
            call_function(ast);
            break;
        case AST_VAR: 
            printf("\n\t");
            //alloc_var(ast);
            break;
        case AST_RET:        
            return_statement(ast->ret_val);
            break;
        default:
            printf("Unrecognized Ast->type\n");
    }
}

void assembly_header()
{
    printf("\nASSEMBLY CODE\n\n");
    printf(".section    __TEXT,__text,regular,pure_instructions\n");
    printf(".macosx_version_min 10, 13\n");
    printf(".intel_syntax noprefix\n");
}

void compile(List *block)
{
    assembly_header();
    for (Iter *i = list_iter(block); !iter_end(i);)
        emit_expr(iter_next(i));
}
