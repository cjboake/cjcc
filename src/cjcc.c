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
    AST_OP,
    AST_PLUS,
    AST_MINUS,
    AST_INT,
    AST_STR
};

typedef struct Ast {
  int type;
  union {
    int ival;
    char *sval;
    struct {
      struct Ast *left;
      struct Ast *right;
    };
  };
} Ast;

typedef struct Token {
    int type;
    char *val;
} Token;

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

Ast *ast_operator(int type, Ast *left, Ast *right)
{
    Ast *o = malloc(sizeof(Ast));
    o->type = type;
    o->left = left;
    o->right = right;
    return o;
}

Ast *ast_string(char buffer[])
{
    Ast *r = malloc(sizeof(Ast)); 
    r->type = AST_STR; 
    r->sval = buffer; 
    return r;
}

Ast *read_string(FILE *fp, char d)
{
    char *buf = malloc(BUFLEN);
    int i = 0;
    for(;;) {
        char c = getc(fp); 
        if(c == EOF)
            break;
        if(c == ' ')
            break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return ast_string(buf);
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
    Ast *node = malloc(sizeof(Ast));
    node->type = op;
    node->left = l;
    node->right = r;
    return node;
}

int skip_space(FILE *fp, char d)
{
    int c;
    while((c = fgetc(fp)) != EOF) {
        if(isspace(c))
            continue;
        else 
            return c;
    }
    return -1;
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

Ast *read_primitive(FILE *fp, int c)
{
    Ast *t = malloc(sizeof(Ast));
    if(isdigit(c)){
        return read_num(fp, c - '0');
    }else if(c == '"'){
        return read_string(fp, c);
    }
    return t;
}

Ast *read_expr2(FILE *fp, Ast *left)
{
    int op;
    int c;
    
    c = fgetc(fp);
    int d = skip_space(fp, c);
    if(d == '+') op = AST_PLUS;
    if(d == '-') op = AST_MINUS;
    int e = skip_space(fp, d);
    Ast *right = read_primitive(fp, e);
    return make_ast_node(left, right, op);
}

void print_single_node(Ast *ast)
{
    printf("Ast Operator -> %d\n", ast->type);
    printf("Ast Left -> %d\n", ast->left->ival);
    printf("Ast Right -> %d\n", ast->right->ival);
}

void print_ast(Ast *ast)
{
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
    case AST_INT:
       printf("%d", ast->ival);
 break;
    case AST_STR:
       printf("%s", ast->sval);
       break;
     default:
       printf("should not reach here\n");
   }
}

Ast *read_expr(FILE *p)
{
    int c = fgetc(p);
    Ast *left = read_primitive(p, c);
    return read_expr2(p, left); 
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
    printf("_main:\n");
}

void emit_intexpr(Ast *ast)
{
    if(ast->type == AST_INT)
        printf("move rax, %d\n", ast->ival);
}

void emit_op(Ast *ast)
{
    char *op;
    if(ast->type == AST_PLUS)
        op = "add";
    if(ast->type == AST_MINUS)
        op = "sub";
    emit_intexpr(ast->left);
    if(ast->right->type == AST_INT)
        printf("mov rbx, %d\n", ast->right->ival);
    printf("%s rax, rbx\n", op);
    printf("mov rbi, 0\n");
    printf("ret\n");
}

void compile(Ast *ast)
{
    assembly_header();
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
    char *input;
    if(argv[1] != NULL) {
        input = argv[1];
        Ast *ast = scan(input);
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
