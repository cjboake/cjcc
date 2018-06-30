#include <stdio.h>
#include <stdlib.h>
#include "cjcc.h"
#include "util.h"
#include "parse.h"
#include "lex.h"

#define MAX_ARGS 6
#define EXPR_LEN 100

Ast *make_ast_operator(int type)
{
    Ast *op = malloc(sizeof(Ast));
    if(type == '+')
        op->type = AST_PLUS;
    else if(type == '-')
        op->type = AST_MINUS; 
    return op;
}

Ast *ast_string(char buffer[])
{
    Ast *r = malloc(sizeof(Ast)); 
    r->type = AST_STR; 
    r->sval = buffer; return r;
}

Ast *make_decl(char *name)
{
    Ast *variable = malloc(sizeof(Ast));
    variable->type = AST_DECL;
    variable->name = name;
    variable->vpos = 1;
    return variable;
}

Ast *make_var(char *name)
{
    Ast *variable = malloc(sizeof(Ast));
    variable->type = AST_VAR;
    variable->name = name;
    variable->vpos = 1;
    return variable;
}

Ast *make_return(Ast *val)
{
    Ast *ret = malloc(sizeof(Ast));
    ret->type = AST_RET;
    ret->ret_val = val;
    return ret;
}

Ast *make_ast_var(Ast *variable, Ast *val)
{
    variable->value = val;
    return variable;
}

Ast *make_ast_func(char *name, int n, Ast **a)
{
    Ast *ast = malloc(sizeof(Ast));
    ast->type = AST_FUNC;
    ast->fname = name;
    ast->nargs = n;
    ast->args = a;
    
    return ast;
}

Ast *make_ast_int(int val)
{
    Ast *r = malloc(sizeof(Ast));
    r->type = AST_INT;
    r->ival = val;
    return r;
}

Ast *make_ast_string(char *str)
{
    Ast *a = malloc(sizeof(Ast));
    a->type = AST_STR; 
    a->sval = str;
    return a; 
}

Ast *make_ast_char(char c)
{
    Ast *a = malloc(sizeof(Ast));
    a->type = AST_CHAR;
    a->sval = &c; 
    return a;
}

Ast *make_ast_node(Ast *l, Ast *r, int op)
{
    Ast *nd = malloc(sizeof(Ast));
    nd->type = op; 
    nd->left = l;
    nd->right = r;
    return nd;
}

int priority(char op)
{
    switch(op) {
        case '+':
            return 0;
        case '-':
            return 1;
        default:
            return -1;
    }
}

Ast *read_func_args(FILE *fp, char *buf)
{
    int i = 0, nargs = 0;
    Ast **args = malloc(sizeof(Ast) * MAX_ARGS + 1);
    for(;i < MAX_ARGS; i++){
        if(i > 6) error("Function exceeds max args\n"); 
        skip_space(fp); 
        Token *tok = read_token(fp);
        if(tok->punct == ')'){
            expect(fp, '{');
            break;
        } else {
            Ast *a = read_primitive(fp, tok);
            args[i] = a;
            if(check_for(',', fp)) continue;
            if(check_for(')', fp)){
                expect(fp, '{');
                break;
            }
        }
        nargs++;
    }
    Ast *a = malloc(sizeof(Ast));
    return make_ast_func(buf, nargs, args);
}

int is_type_keyword(Token *tok)
{
    int r = 0;
    if(tok->type != TTYPE_IDENT)
        return r;
    char *keywords[] = { "int", "char" };
    for(int i=0; i < 3; i++) {
        if(!strcmp(keywords[i], tok->sval)){
            return r = 1;  
        }
    }
    return r;
}

Ast *read_decl(FILE *fp)
{
    fseek(fp, -1L, SEEK_CUR);
    Token *tok = read_token(fp);
    char *name = tok->sval;
    Ast *a = make_ast_var(make_decl(name), rd_expr2(fp)); 
    return a;
}

int is_token_ident(Token *tok, char *ident)
{
    return tok->type == TTYPE_IDENT && !strcmp(tok->sval, ident);
}

Ast *read_var(FILE *fp, Token *tok)
{
    char *name = tok->sval;
    return make_ast_var(make_var(name), rd_expr2(fp)); 
}

Ast *rd_statement(FILE *fp, Token *tok)
{
    if(is_token_ident(tok, "return")){ 
        fseek(fp, -1L, SEEK_CUR);
        Ast *r = rd_expr2(fp);
        Ast *mr = make_return(r);
        return mr;
    } else { 
        return read_var(fp, tok);
    }
}

Ast *func_or_ident(FILE *fp, Token *tok)
{
    char *name = tok->sval;
    skip_space(fp);
    if(check_for('(', fp)){
        return read_func_args(fp, name); 
    } 
    return is_type_keyword(tok) ? read_decl(fp) : rd_statement(fp, tok); 
}

Ast *read_primitive(FILE *fp, Token *tok)
{
    switch(tok->type){
        case TTYPE_INT:
            return make_ast_int(tok->ival);
        case TTYPE_PUNCT:
            return make_ast_operator(tok->punct);
        case TTYPE_STRING:
            return make_ast_string(tok->sval);
        case TTYPE_IDENT:
            return func_or_ident(fp, tok);
        case TTYPE_CHAR:
            return make_ast_char(tok->c);
    }
    return NULL;
}

int find_var(char *name, Ast **fbod)
{
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(!strcmp(name, fbod[i]->name)){
            return 1;
        }
    }
    return 0; 
}

int get_var_pos(char *name, Ast **fbod)
{
    printf("get_var_pos\n");
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(!strcmp(name, fbod[i]->name)){
            printf("The position we want: %d", fbod[i]->vpos);
            return fbod[i]->vpos;
        }
    }
    error("Could not find previously declared var");
    return -1;
}

Ast *make_fn(Ast *f, FILE *fp)
{
    Ast **fbod = malloc(sizeof(Ast) * MAX_ARGS + 1);
    for(int i = 0; i < EXPR_LEN; i++){
        Ast *a = rd_expr2(fp);       
        if(!a){
            f->body = fbod; 
            break; 
        }       
        int d = find_var(a->name, fbod);
        if(a->type == AST_DECL && !d) a->vpos = i+1;
        if(a->type == AST_VAR) a->value->vpos = get_var_pos(a->name, fbod);   
        if(a->type == AST_RET)
           // gonna need to make something that 
           // goes the AST_RET node and assigns
           // positions from the vars.
        fbod[i] = a;
        //if(check_for('}', fp)) break;
    }
    return f;
}

Ast *make_arith_expr(Ast *left, Ast *op, FILE *fp)
{
    op->left = left;
    Ast *right = rd_expr2(fp);
    op->right = right;
    return op;
}

Ast *rd_expr2(FILE *fp)
{
    //skip_space(fp);
    Token *tok = read_token(fp);
    if(tok == NULL){ 
        return NULL;
    }
    if(tok->type == TTYPE_PUNCT || tok == NULL){ 
        unget_token(fp, tok);
        return NULL;
    }
    Ast *ast = read_primitive(fp, tok);
    if(!ast) return NULL;
    if(ast->type == AST_FUNC){
        ast = make_fn(ast, fp);
        return ast;
    }
    if(ast->type == AST_RET){
        return ast; 
    }
    if(ast->type == AST_PLUS){
        return make_ast_operator(ast->type);
    }
    if(ast->type == AST_INT){
        if(check_for(';', fp)) return ast;
        if(check_for('+', fp)){
            Ast *op = make_ast_operator('+');
            return make_arith_expr(ast, op, fp);
        } else if(check_for('-', fp)){
            Ast *op = make_ast_operator('-');
            return make_arith_expr(ast, op, fp); 
        }
    }
    if(ast->type == AST_DECL){
        skip_space(fp);
        expect(fp, '=');
        ast->value = rd_expr2(fp);
        
        return ast; 
    }
    if(ast->type == AST_VAR){
        skip_space(fp);
        if(check_for('=', fp)){ 
            return ast->value = rd_expr2(fp);
        }
        if(check_for('+', fp)){
            Ast *op = make_ast_operator('+');
            return make_arith_expr(ast, op, fp); 
        } else if(check_for('-', fp)){
            Ast *op = make_ast_operator('-');
            return make_arith_expr(ast, op, fp); 
        } else if(check_for(';', fp)){
            return ast;
        }
    }
    skip_space(fp);
    int d = fgetc(fp);
    if(d == ';'){
        return ast;
    }
    if(d == EOF) return NULL;
    Ast *right = rd_expr2(fp);
    Ast *ret = make_ast_node(ast, right, d);
    return ret;
}

Ast *read_expr(FILE *fp)
{
    Ast *a = malloc(sizeof(Ast));
    a = rd_expr2(fp);
    return a; 
}
