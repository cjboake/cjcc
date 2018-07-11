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

Ast *make_decl(char *name, int t)
{
    Ast *variable = malloc(sizeof(Ast));
    variable->ctype = t; 
    variable->type = AST_DECL;
    variable->name = name;
    variable->vpos = 1;
    return variable;
}

Ast *make_decln(char *name)
{
    Ast *variable = malloc(sizeof(Ast));
    variable->type = AST_DECLN;
    variable->name = name;
    variable->vpos = 1;
    return variable;
}

Ast *make_ref(char *name)
{    
    Ast *variable = malloc(sizeof(Ast));
    variable->type = AST_REF;
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

Ast *make_ast_func(char *name, int n, Ast **a, int rtype)
{
    Ast *ast = malloc(sizeof(Ast));
    ast->type = AST_FUNC;
    ast->return_type = rtype;
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

Ast *read_func_args(FILE *fp, char *buf, int type)
{
    int i = 0, nargs = 0;
    Ast **args = malloc(sizeof(Ast) * MAX_ARGS + 1);
    for(;i < MAX_ARGS; i++){
        if(i > 6) error("Function exceeds max args\n"); 
        //skip_space(fp); 
        Token *tok = read_token(fp);
        if(tok->punct == ')'){
            expect(fp, '{');
            break;
        } else {
            Ast *a = read_primitive(fp, tok);
            args[i] = a;
            nargs++;
            if(check_for(',', fp)) continue;
            if(check_for(')', fp)){
                expect(fp, '{');
                break;
            }
        }
    }
    return make_ast_func(buf, nargs, args, type);
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

int get_type(Token *t)
{
    switch(atoi(t->sval)){
        case INT:
             return INT;
        case STRING:
             return STRING;
        case POINTER:
             return POINTER;
        case ARRAY:
             return ARRAY;
        case CHAR:
             return CHAR;
        default:
             return -1;
    }
}

int is_pointer(char *name)
{
    if(name[0] == '*')
        return 1; 
    return 0;
}

Ast *read_decl(FILE *fp, Token *t)
{
    //fseek(fp, -1L, SEEK_CUR);
    skip_space(fp);
    Token *tok = read_token(fp);
    if(tok == NULL) p("tok was null\n");
    char *name = tok->sval;
    int type = get_type(t);
    
    if(check_for('(', fp)) return read_func_args(fp, name, type); 
    Ast *node = make_ast_var(make_decl(name, type), rd_expr2(fp)); 
    if(is_pointer(node->name)){
        node->pointer = 1;
    }
    return node;
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
    return is_type_keyword(tok) ? read_decl(fp, tok) : rd_statement(fp, tok); 
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

int get_vpos(char *name, Ast **fbod)
{
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(!strcmp(name, fbod[i]->name)){
            return fbod[i]->vpos;
        }
    }
    error("Could not find previously declared var");
    return -1;
}

int is_operator(Ast *node){
    if(node->type == AST_PLUS || node->type == AST_MINUS) 
        return 1;
    return 0;
}

void ret_pos(Ast *node, Ast **fbod)
{
    if(is_operator(node)){
        if(node->left != NULL) ret_pos(node->left, fbod);
        if(node->right != NULL) ret_pos(node->right, fbod);
    }
    if(node->type == AST_VAR)
        node->vpos = get_vpos(node->name, fbod);
}

void handle_pointer(Ast *a, Ast **fbod)
{
    ensure_ptr(a->value); 
    if(!find_var(a->value->name, fbod)) 
        error("Pointer does not reference valid var.");
}

Ast *make_fn(Ast *f, FILE *fp)
{
    int d;
    Ast **fbod = malloc(sizeof(Ast) * MAX_ARGS + 1);
    for(int i = 0; i < EXPR_LEN; i++){
        int pos = i+1+f->nargs;
        Ast *a = rd_expr2(fp);       
        if(!a){
            f->body = fbod; 
            break; 
        }       
        if(a->pointer == 1){
            handle_pointer(a, fbod);
            a->value->vpos = pos;
            a->value->ref_pos = get_vpos(a->value->name, fbod);  
        }else{
            if(a->type != AST_RET) d = find_var(a->name, fbod);
            if(a->type == AST_DECL && !d) a->vpos = pos;
            if(a->type == AST_VAR) a->value->vpos = get_vpos(a->name, fbod);   
            if(a->type == AST_RET) ret_pos(a->ret_val, fbod); 
        }
        fbod[i] = a;
        //if(check_for('}', fp)) break;
    }
    expect(fp, '}');
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
    skip_space(fp);
    Token *tok = read_token(fp);
    skip_space(fp);
    if(tok == NULL) return NULL;
    if(tok->type == TTYPE_PUNCT){ 
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
        if(check_for(';', fp)){
            return ast;
        }
        if(check_for('=', fp)){ 
            return ast->value = rd_expr2(fp);
        }else if(check_for('+', fp)){
            Ast *op = make_ast_operator('+');
            return make_arith_expr(ast, op, fp); 
        }else if(check_for('-', fp)){
            Ast *op = make_ast_operator('-');
            return make_arith_expr(ast, op, fp); 
        }  
    }
    skip_space(fp);
    int d = fgetc(fp);
    if(d == ';'){
        return ast;
    }
    if(d == EOF){ 
        return NULL;
    }
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
