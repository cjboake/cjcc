#include <stdio.h>
#include <stdlib.h>
#include "cjcc.h"
#include "util.h"
#include "parse.h"
#include "lex.h"
#include "list.h"

#define MAX_ARGS 6
#define EXPR_LEN 100

List *FUNCTIONS = EMPTY_LIST;

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
    list_append(FUNCTIONS, name); 
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

Ast *make_ast_func_ref(char *name, Ast **arg)
{
    Ast *a = malloc(sizeof(Ast));
    a->type = AST_REF;
    a->fname = name;
    a->args = arg;
    return a;
}

Ast *make_ast_if(Ast *cond, List *then, List *els)
{
    Ast *r = malloc(sizeof(Ast));
    r->type = AST_IF;
    r->cond = cond;
    r->then = then;
    r->els = els;
    return r;
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
            args[i]->vpos = nargs;
            if(check_for(',', fp)) continue;
            if(check_for(')', fp)){
                expect(fp, '{');
                break;
            }
        }
    }
    return make_ast_func(buf, nargs, args, type);
}

Ast *read_ref_args(FILE *fp, char *name)
{
    Ast **args = malloc(sizeof(Ast) * MAX_ARGS + 1);
    int i = 0;
    for(;i < MAX_ARGS; i++){
        Token *tok = read_token(fp);
        if(tok->punct == ',') tok = read_token(fp); 
        if(tok->punct == ')' || tok->punct == ';'){ 
            if(tok->punct == ')') expect(fp, ';');
            break;
        }
        Ast *a = read_primitive(fp, tok);
        args[i] = a;
    }
    return make_ast_func_ref(name, args);
}

int is_type_keyword(Token *tok)
{
    int r = 0;
    if(tok->type != TTYPE_IDENT)
        return r;
    char *keywords[] = { "int", "char", "void" };
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
        node->type = AST_PTR;
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

int existing_func(char *name)
{
    for (Iter *i = list_iter(FUNCTIONS); !iter_end(i);) {
        char *func = iter_next(i); 
        if(!strcmp(name, func)){ 
            return 1;
        }
    }
    p("func did not exist");
    return 0;
}

Ast *handle_if(FILE *fp)
{
    expect(fp, '(');
    Ast *cond = rd_expr2(fp);
    expect(fp, '{');
    List *then = read_block(fp);
    expect(fp, '}'); 
    
    // We might not need to return both
    // Rather, evaluate the condition
    // Then return the relevant block of code
    
    return make_ast_if(cond, then, NULL);
}

Ast *rd_statement(FILE *fp, Token *tok)
{
    Ast *a = malloc(sizeof(Ast)); 
    if(is_token_ident(tok, "return")){ 
        fseek(fp, -1L, SEEK_CUR);
        Ast *r = rd_expr2(fp);
        Ast *mr = make_return(r);
        return mr;
    } else if(is_token_ident(tok, "if")){
        return handle_if(fp);    
    } else if(check_for('(', fp) && existing_func(tok->sval)){
        char *name = tok->sval;
        return read_ref_args(fp, name);
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

int find_var(char *name, Ast **fbod, Ast **args)
{
    int a = 0;
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(!strcmp(name, fbod[i]->name)){
            return 1;
        }
    }
    for(int i = 0; i < 100; i++){
        if(!args[i]) break;
        if(!strcmp(name, args[i]->name)){
            return 1;
        }
    }
    return 0; 
}

int get_vpos(char *name, Ast **fbod, Ast **args)
{
    for(int i = 0; i < 100; i++){
        if(!fbod[i]) break;
        if(!strcmp(name, fbod[i]->name)){
            return fbod[i]->vpos;
        }
    }
    for(int i = 0; i < 100; i++){
        if(!args[i]) break;
        if(!strcmp(name, args[i]->name)){
            return args[i]->vpos;
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

void ret_pos(Ast *node, Ast **fbod, Ast **args)
{
    if(is_operator(node)){
        if(node->left != NULL) ret_pos(node->left, fbod, args);
        if(node->right != NULL) ret_pos(node->right, fbod, args);
    }
    if(node->type == AST_VAR){
        node->vpos = get_vpos(node->name, fbod, args);
    }
}

void handle_pointer(Ast *a, Ast **fbod, Ast **args)
{
    ensure_ptr(a->value); 
    if(!find_var(a->value->name, fbod, args)) 
        error("Pointer does not reference valid var.");
}

// modify to recurse expression
int check_declaration(Ast *ast)
{
    int s = -1;
    int n = ast->value->type == AST_PLUS || ast->value->type == AST_MINUS;
    if(ast->value->left != NULL && ast->value->right != NULL)
        s = ast->value->left->type == AST_VAR && ast->value->right->type == AST_VAR;
    if(n && s)
       return 1;
    return 0;
}

void assign_varpos(Ast *ast, Ast **args, Ast **fbod)
{
    ast->left->vpos = get_vpos(ast->left->name, fbod, args); 
    ast->right->vpos = get_vpos(ast->right->name, fbod, args); 
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
        if(a->type == AST_PTR){
            handle_pointer(a, fbod, f->args);
            a->value->vpos = pos;
            a->value->ref_pos = get_vpos(a->value->name, fbod, f->args);  
        }else{
            if(a->type != AST_RET && a->type != AST_IF) d = find_var(a->name, fbod, f->args);
            if (a->type == AST_REF){ 
                expect(fp, ';'); 
            }
            if(a->type == AST_DECL && !d){ 
                if(check_declaration(a)) assign_varpos(a->value, f->args, fbod); 
                a->vpos = pos; 
            }
            if(a->type == AST_VAR){ 
                a->value->vpos = get_vpos(a->name, fbod, f->args);   
            }
            if (a->type == AST_RET){ 
                ret_pos(a->ret_val, fbod, f->args);
            }
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
    if(ast->type == AST_IF){
        return ast;
    }
    if(ast->type == AST_FUNC){
        ast = make_fn(ast, fp);
        return ast;
    }
    if(ast->type == AST_RET){
        return ast; 
    }
    if(ast->type == AST_REF){
        return ast; 
    }
    if(ast->type == AST_PLUS){
        return make_ast_operator(ast->type);
    }
    if(ast->type == AST_INT){
        if(check_for(';', fp)){ 
            return ast;
        }
        if(check_for(')', fp)){
            return ast;
        }
        if(check_for('+', fp)){
            Ast *op = make_ast_operator('+');
            return make_arith_expr(ast, op, fp);
        } else if(check_for('-', fp)){
            Ast *op = make_ast_operator('-');
            return make_arith_expr(ast, op, fp); 
        } else if(check_for('<', fp)){
            printf("ayyy less than!\n");
            Ast *op = make_ast_operator('<');
            return make_arith_expr(ast, op, fp);
        } else if(check_for('>', fp)){
            printf("ayyy greater than!\n");
            Ast *op = make_ast_operator('>');
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
        if(check_for(')', fp)){
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
        } else if(check_for('<', fp)){
            Ast *op = make_ast_operator('<');
            return make_arith_expr(ast, op, fp);
        } else if(check_for('>', fp)){
            Ast *op = make_ast_operator('>');
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
