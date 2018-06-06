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
#define MAX_ARGS 6

enum {
    AST_PLUS,
    AST_MINUS,
    AST_INT,
    AST_STR, 
    AST_CHAR,
    AST_FUNC
};

enum {
    TTYPE_IDENT,
    TTYPE_PUNCT,
    TTYPE_INT,
    TTYPE_CHAR,
    TTYPE_STRING
};

typedef struct {
    int type;
    union {
        int ival;
        char *sval;
        char punct;
        char c;
    };
} Token;

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

Ast *read_expr(FILE *p);
Ast *read_primitive(FILE *fp, Token *tok);
void print_ast(Ast *ast);
Ast *rd_expr2(FILE *fp);
void skip_space(FILE *fp); 
Token *read_token(FILE *fp);

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
        skip_space(fp);
        Token *tok = read_token(fp);
        if(tok->punct == ')')
            break;
    }
    Ast *a = malloc(sizeof(Ast));
    return make_ast_func(buf, 0, args);
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
    //fseek(fp, -1L, SEEK_CUR); 
    buf[0] = d;
    int i = 1;
    for(;;) {
        char c = getc(fp); 
        if(!isalnum(c) || c == '"'){
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
        return make_ast_var(make_var(name));
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

void print_nd(Ast *ast)
{
    printf("In print_nd ->> ");
    if(ast->type == AST_FUNC) printf("ast func: %s\n", ast->fname);
    if(ast->type == AST_INT) printf("ast int: %d\n", ast->ival);
    if(ast->type == AST_PLUS) printf("ast +: %d\n", ast->type);
    if(ast->type == AST_MINUS) printf("ast -: %d\n", ast->type);
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
    f->body = rd_expr2(fp); 
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
    Ast *ast = read_primitive(fp, tok);
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

Token *read_string(FILE *fp, char ch)
{
    Token *str_tok = read_ident(fp, ch);
    return str_tok; 
}

Token *read_token(FILE *fp)
{
    skip_space(fp);
    int c = fgetc(fp);
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
        case ',': case ';':
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

void printf_func(Ast *a)
{
    printf("AST-> %s\n", a->fname);
    printf("AST->BODY->TYPE-> %c\n", a->body->type);
    printf("AST->BODY->LEFT-> %d\n", a->body->left->ival);
    printf("AST->BODY->RIGHT-> %d\n", a->body->right->ival);
}

Ast *read_expr(FILE *fp)
{
    Ast *a = malloc(sizeof(Ast));
    a = rd_expr2(fp);
    fseek(fp, -3L, SEEK_CUR); 
    int c = fgetc(fp);
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
