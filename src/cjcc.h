#ifndef CJCC_H
#define CJCC_H

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

#endif /* CJCC.H */
