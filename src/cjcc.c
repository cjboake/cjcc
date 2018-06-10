#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"

#define BUFLEN 256
#define MAX_ARGS 6
#define EXPR_LEN 100

void check_file(FILE *p)
{
    if(p == NULL) {
        printf("The file failed to open.\n");
        exit(0);
    }
}

int fpeek(FILE *stream)
{
    int c;
    c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

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

Ast *make_ast_node(Ast *l, Ast *r, int op)
{
    Ast *nd = malloc(sizeof(Ast));
    nd->type = op; 
    nd->left = l;
    nd->right = r;
    return nd;
}

void unget_token(FILE *fp, Token *tok)
{
    ungetc(tok->punct, fp);    
}

void error(char *input)
{
    fprintf(stderr, "%s", input);
    exit(0);
}

void expect(FILE * fp, int c)
{
    Token *tok = read_token(fp);
    if(tok->punct != c){
        fprintf(stderr, "Error, did not get expected token '%c'.\n", c);
        exit(0);
    }
}

int check_for(char c, FILE *fp)
{
    Token *tok = read_token(fp);
    if(tok->punct == c){
        return 1; 
    } else {
        unget_token(fp, tok);
        return 0;
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

Token *make_string_tok(char *string)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_IDENT;
    tok->sval = string; 
    return tok;
}

Token *read_ident(FILE *fp, char d)
{
    char *buf = malloc(BUFLEN);
    buf[0] = d;
    int i = 1;
    for(;;) {
        char c = getc(fp); 
        if(!isalnum(c) || c == '"' || c == ')'){
            ungetc(c, fp);
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return make_string_tok(buf);
}

void skip_space(FILE *fp)
{
    int c;
    while((c = fgetc(fp)) != EOF) {
        if(isspace(c)){
            continue;
        }else{ 
            ungetc(c, fp);
            break;
        }
    }
}

Ast *func_or_ident(FILE *fp, Token *tok)
{
    char *name = tok->sval;
    skip_space(fp);
    Token *t = read_token(fp);
    if(t->punct == '('){
        return read_func_args(fp, name); 
    } else {
        unget_token(fp, t);
        // produce ast->name & ast->var->holds whatver val
        // ex: ast->var->ival || ast->var-type (+)
        return make_ast_var(make_var(name), rd_expr2(fp)); 
    }
}

Token *make_punc_tok(int c)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_PUNCT;
    tok->punct = c;
    return tok;
}

Token *make_int_tok(int n)
{
    Token *r = malloc(sizeof(Token));
    r->type = TTYPE_INT; 
    r->ival = n;
    return r;
}

Token *read_num(FILE *fp, int n)
{
    for(;;){
        int c = getc(fp);    
        if(!isdigit(c) || c == ';'){
            if(c == ';'){
                ungetc(c, fp);
                Token *tok = make_int_tok(n);
                return tok;
            }
            Token *tok = make_int_tok(n);
            return tok;
        } // else if c == something else, untermed? 
        n = n * 10 + (c - '0');
    }
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

Ast *make_ast_char(char c)
{
    Ast *a = malloc(sizeof(Ast));
    a->type = AST_CHAR;
    a->sval = &c; 
    return a;
}

Ast *make_ast_string(char *str)
{
    Ast *a = malloc(sizeof(Ast));
    a->type = AST_STR; 
    a->sval = str;
    return a; 
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
        fbod[i] = a;
        if( 0 ){  //check_for('}', fp)){
            f->body = fbod; 
            break;
        }
    } 
    if(f->body[0] != NULL)
        printf("f->body[0]->name: %s, f->body[0]->val: %d\n", f->body[0]->name, f->body[0]->var->ival);
    if(f->body[1] != NULL)
        printf("f->body[1]->name: %s, f->body[1]->val: %d\n", f->body[1]->name, f->body[1]->var->ival);
    if(f->body[2] != NULL)
        printf("f->body[2]->name: %s, f->body[2]->type: %c, f->body[2]->var->left->ival: %d ", f->body[2]->name, f->body[2]->var->type, f->body[2]->var->left->ival);
    printf("f->body[2]->var-right->ival: %d\n", f->body[2]->var->right->ival);
    
    return f;
}

Token *make_char_tok(int ch)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = TTYPE_CHAR;
    tok->c = ch;
    return tok;
}

Token *read_char(FILE *fp, int ch)
{
    int c = fgetc(fp);
    return make_char_tok(c);
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

Token *read_string(FILE *fp, char ch)
{
    Token *str_tok = read_ident(fp, ch);
    return str_tok; 
}

Token *read_token(FILE *fp)
{
    skip_space(fp);
    int c = fgetc(fp);
    if(c == EOF) error("Unexpected EOF\n");
    switch(c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return read_num(fp, c - '0');
        case '"':
            return read_string(fp, c);
        case '\'':
            return read_char(fp, c);
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B':
        case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W':
        case 'X': case 'Y': case 'Z': case '_':
            return read_ident(fp, c);
        case '/': case '=': case '*': case '+': case '-': case '(': case ')':
        case ',': case ';': case '{': case '}':
            return make_punc_tok(c);
        case EOF:
            return NULL;
        default:
            printf("Unexpected Character %c", c);
    }
    return NULL;
}

void print_ast(Ast *ast)
{
    printf("-> Print AST : %d\n", ast->type);
    switch (ast->type) {
        case AST_PLUS:
            printf("(+ ");
            goto print_op;
        case AST_MINUS:
            printf("(- ");
        print_op:
            print_ast(ast->left);
            printf(" ");
            print_ast(ast->right);
            printf(")");
            break;
        case AST_FUNC:
            printf("%s(", ast->fname);
            printf(")");
            //print_ast(ast->body);
            break;
        case AST_INT:
            printf("%d", ast->ival);
            break;
        case AST_STR:
            printf("%s", ast->sval);
            break;
        case AST_VAR:
            printf("%s", ast->name);
            print_ast(ast->var);
        default:
            printf("( %c", ast->type);
            print_ast(ast->left);
            printf(" ");
            print_ast(ast->right);
            printf(" )");
    }
}

Ast *read_expr(FILE *fp)
{
    Ast *a = malloc(sizeof(Ast));
    a = rd_expr2(fp);
    return a; 
}

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
        printf("In the new AST_VAR block\n"); 
        emit_intexpr(ast); 
    }
    if(ast->type == AST_INT) {     
        emit_intexpr(ast); 
    }
    if(ast->type == AST_PLUS || ast->type == AST_MINUS) {
        emit_op(ast); 
    }
}

Ast *scan(char *input)
{
    FILE *fp;
    fp = fopen(input, "r");
    check_file(fp);
    Ast *ast = read_expr(fp);
    return ast;
}

void run(char *argv[])
{
    FILE *fp;
    int r = 0, i = 0, p = 0;
    char *input;
    Ast *exprs[EXPR_LEN];
    if(argv[2] != NULL && !strcmp(argv[2], "-a")) p = 1;
    if(argv[1] != NULL) {
        input = argv[1];
        Ast *ast = scan(input);
        //compile(ast);
    } else {
        input = "Please give an input\n";
        printf("%s", input);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    run(argv);
    return 0;
}
