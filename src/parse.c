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

Ast *make_var(char *name)
{
    Ast *varr = malloc(sizeof(Ast));
    varr->type = AST_VAR;
    varr->name = name;
    varr->vpos = 1;
    return varr;
}

Ast *make_ast_var(Ast *varr, Ast *val)
{
    varr->var = val;
    return varr;
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

int is_keyword(Token *tok)
{
    int r = 0;
    if(tok->type != TTYPE_IDENT)
        return r;
    char *keywords[] = { "int", "return" };
    for(int i=0; i < 3;i++) {
        if(!strcmp(keywords[i], tok->sval)){
            return r = 1;  
        }
    }
    return r;
}

int find_var(char *name, Ast **fbod)
{
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(strcmp(name, fbod[i]->name))
            return 1;
    }
    return 0; 
}

Ast *func_or_ident(FILE *fp, Token *tok)
{
    char *name = tok->sval;
    skip_space(fp);
    Token *t = read_token(fp);
    if(t->punct == '('){
        return read_func_args(fp, name); 
    } else {
        // produce ast->name & ast->var->holds whatver val
        // ex: ast->var->ival || ast->var-type (+)
        return is_keyword(tok) ? make_ast_var(make_var(name), rd_expr2(fp)) : rd_expr2(fp); 
    }
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

Ast *make_fn(Ast *f, FILE *fp)
{
    Ast **fbod = malloc(sizeof(Ast) * MAX_ARGS + 1);
    for(int i = 0; i < EXPR_LEN; i++){
        Ast *a = rd_expr2(fp);       
        if(!a){
            f->body = fbod; 
            break; 
        }        
        if(a->type == AST_VAR) a->var->vpos = i+1;
        fbod[i] = a;
    } 
    return f;
}

Ast *rd_expr2(FILE *fp)
{
    skip_space(fp);
    Token *tok = read_token(fp);
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
    if(ast->type == AST_INT){
        if(check_for(';', fp)) return ast;
    } 
    if(ast->type == AST_VAR){
        skip_space(fp);
        expect(fp, '=');
        ast->var = rd_expr2(fp);
        return ast;
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



// test
