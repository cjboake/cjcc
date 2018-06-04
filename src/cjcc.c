#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RETURN "return"
#define INT "int"
#define MAIN "main"
#define ARGV "argv"
#define ARGC "argc"
#define CHAR "char"

#define BUFLEN 256
#define SYM 14

int iterator = 0;
const char *keywords[] = { RETURN, INT, MAIN, ARGV, ARGC, CHAR };

const char symbols[] = { '"', ',', '{', '}', '.', '#', '<', '>','+', '-', '(', ')', ';' };

const char lines[] = { '\n', '\t' };

enum {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE
};

enum {
    NUM,
    SYMB,
    CH
};

enum {
    AST_PLUS,
    AST_MINUS,
    AST_INT,
    AST_STR, 
    AST_FUNC
};

typedef struct Var {
    char *name;
} Var;

typedef struct Ast {
  int type;
  struct Ast *left;
  struct Ast *right;
  union {
    Var *var;
    int ival;
    char *sval;
    struct {
        char *fname;
        int nargs;
        struct Ast **args;
        struct Ast *body;
    };
  };
} Ast;

typedef struct Token {
    int type;
    char *val;
} Token;

Ast *read_expr(FILE *p);
Ast *read_primitive(FILE *fp, int c);
void print_ast(Ast *ast);
Ast *rd_expr2(FILE *fp);

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

int check_white(char c)
{
    int r = 0;
    if(r == ' ')
        r = 1;
    return r;
}

int is_keyword(char *word)
{
    int r = 0; 
    int i = 0;
    for(i = 0; i < 7; i++) {
       if(strcmp(word, keywords[i]))
           r = 1;
    }
    return r;
}

int is_symbol(char c)
{
    int r = 0; 
    int i = 0;
    for(i = 0; i < SYM; i++) {
       if(c == symbols[i])
           r = 1;
    }
    return r;
}

int read_input(char c)
{
    char *buf;
    int r = 0;
    int t = 0; 
    if(c == '\n' || c == '\t')
        printf("This is either a newline or tab.\n");
    if(is_symbol(c))
        r = SYMB;
    if(isalpha(c)) 
        r = CH;
    if(isdigit(c))
        r = NUM;
    return r;
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
    r->sval = buffer; 
    return r;
}

Var *make_var(char *n)
{
    Var *v = malloc(sizeof(Var));
    v->name = n;
    return v;
}

Ast *make_ast_var(Var *v)
{
    Ast *ast = malloc(sizeof(Ast));
    ast->var = v;
    return ast;
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

Ast *make_ast_head(Ast *func, FILE *fp)
{
    Ast *ast = malloc(sizeof(Ast));
    ast->type = func->type;
    ast->fname = func->fname;
    ast->left = read_expr(fp);
    return ast;
}

Ast *make_ast_node(Ast *l, Ast *r, int op)
{
    Ast *nd = malloc(sizeof(Ast));
    nd->type = op; 
    nd->left = l;
    nd->right = r;
    return nd;
}

Ast *read_func_args(FILE *fp, char *buf)
{
    Ast **args = malloc(sizeof(Ast));
    for(;;){
        int c = fgetc(fp);   
        if(c == ')')
            break;
    }
    return make_ast_func(buf, 0, args);
}

char *read_ident(FILE *fp, char d)
{
    char *buf = malloc(BUFLEN);
    fseek(fp, -1L, SEEK_CUR); 
    buf[0] = d;
    int i = 1;
    for(;;) {
        char c = getc(fp); 
        if(!isalnum(c)){
            ungetc(c, fp);
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
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

Ast *func_or_ident(FILE *fp, char d)
{
    char *name = read_ident(fp, d);
    skip_space(fp);
    int c = fgetc(fp);
    if(c == '(')
        return read_func_args(fp, name); 
    else
        return make_ast_var(make_var(name));
}

Ast *read_num(FILE *fp, int n)
{
    for(;;){
        int c = getc(fp);    
        if(!isdigit(c)){
            ungetc(c, fp);        
            Ast *t = make_ast_int(n);
            return t;
        } 
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

void print_nd(Ast *ast)
{
    printf("In print_nd ->> ");
    if(ast->type == AST_FUNC) printf("ast func: %s\n", ast->fname);
    if(ast->type == AST_INT) printf("ast int: %d\n", ast->ival);
    if(ast->type == AST_PLUS) printf("ast +: %d\n", ast->type);
    if(ast->type == AST_MINUS) printf("ast -: %d\n", ast->type);
}   


Ast *read_primitive(FILE *fp, int c)
{
    int pr = priority(c); 
    int d = fgetc(fp);
    Ast *t = malloc(sizeof(Ast));
    if(isdigit(c)){
        return read_num(fp, c - '0');
    }else if(isalpha(c)){
        return func_or_ident(fp, c);
    }else if(c == '"'){
        printf("tmp\n");
    }else if(pr >= 0){
        return make_ast_operator(pr);            
    }
    return t;
}

Ast *make_fn(Ast *f, FILE *fp)
{
    f->body = rd_expr2(fp); 
    return f;
}

Ast *rd_expr2(FILE *fp)
{
    skip_space(fp);
    int c = fgetc(fp);
    Ast *ast = read_primitive(fp, c);

    if(ast->type == AST_FUNC){
        ast = make_fn(ast, fp);
        return ast;
    }

    skip_space(fp);
    int d = fgetc(fp);
    if(d == EOF){
        return ast;
    }
    Ast *right = rd_expr2(fp);
    Ast *ret = make_ast_node(ast, right, d);
    return ret;
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
            print_ast(ast->body);
            break;
        case AST_INT:
            printf("%d", ast->ival);
            break;
        case AST_STR:
            printf("%s", ast->sval);
            break;
        default:
            printf("( %c", ast->type);
            print_ast(ast->left);
            printf(" ");
            print_ast(ast->right);
            printf(" )");
    }
}

Ast *read_expr(FILE *p)
{
    Ast *a = malloc(sizeof(Ast));
    a = rd_expr2(p);
    printf("AST-> %s\n", a->fname);
    printf("AST->BODY->TYPE-> %c\n", a->body->type);
    printf("AST->BODY->LEFT-> %d\n", a->body->left->ival);
    printf("AST->BODY->RIGHT-> %d\n", a->body->right->ival);
    return a; 
}

Ast *scan(char *input)
{
    char buffer[BUFLEN];
    FILE *fp;
    fp = fopen(input, "r");
    check_file(fp);
    Ast *ast = read_expr(fp);
    return ast;
}

void assembly_header()
{
    printf("ASSEMBLY CODE\n\n");
    printf(".section    __TEXT,__text,regular,pure_instructions\n");
    printf(".macosx_version_min 10, 13\n");
    printf(".intel_syntax noprefix\n");
    printf(".globl  _main\n");
    printf(".p2align    4, 0x90\n\n");
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

void emit_func(Ast *ast)
{
    printf("_%s:\n", ast->fname);
    printf("\tpush rbp\n");
    if(ast->body->type == '+' || ast->body->type == '-') 
        emit_op(ast->body);
}

void compile(Ast *ast)
{
    assembly_header();
    if(ast->type == AST_FUNC) {     
        emit_func(ast); 
    }
    if(ast->type == AST_INT) {     
        emit_intexpr(ast); 
    }
    if(ast->type == AST_PLUS || ast->type == AST_MINUS) {
        emit_op(ast); 
    }
}

void run(char *argv[])
{
    int r = 0;
    int p = 0;
    char *input;
    if(argv[2] != NULL && !strcmp(argv[2], "-a"))
        p = 1;
    if(argv[1] != NULL) {
        input = argv[1];
        Ast *ast = scan(input);
        if(p == 1) {
            printf("-> Print AST <-\n");
            print_ast(ast);
            printf("\n\n");
        }
        compile(ast);
    } else {
        input = "Please give an input\n";
        printf("%s", input);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    printf("-> cjcc is running <-\n\n");
    run(argv);
    return 0;
}
