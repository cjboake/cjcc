#ifndef CJCC_H
#define CJCC_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct Ast {
  int type;
  union {
    // Integer
    int ival;
    // String
    struct {
      char *sval;
    };
    // Variable
    struct {
      int pointer;
      int ref_pos;
      int ctype;
      char *name;
      int vpos;
      struct Ast *value;
    };
    // Binary operator
    struct {
      struct Ast *left;
      struct Ast *right;
    };
    // Function call
    struct {
      char *fname;
      int nargs;
      int return_type;
      struct Ast **args;
      struct Ast **body;
    };
    // Declaration
    struct {
      struct Ast *decl_var;
      struct Ast *decl_init;
    };
    // Return value
    struct {
      struct Ast *ret_val;
    };
  };
} Ast;

typedef struct Type {
    int type;
    Ast *pointer;
} Type;

enum {
    AST_PLUS,
    AST_MINUS,
    AST_INT,  // 2
    AST_STR, 
    AST_CHAR,
    AST_FUNC,
    AST_VAR, // 6
    AST_DECL,
    AST_RET,
    AST_DECLN, // 9
    AST_REF
};

enum {
    TTYPE_IDENT,
    TTYPE_PUNCT,
    TTYPE_INT,
    TTYPE_CHAR,
    TTYPE_STRING
};

enum {
    INT,
    CHAR,
    STRING,
    ARRAY,
    POINTER
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

void run(char *argv[]);
Ast *read_expr(FILE *p);
Ast *read_primitive(FILE *fp, Token *tok);
void print_ast(Ast *ast);
Ast *rd_expr2(FILE *fp);
void skip_space(FILE *fp); 
Token *read_token(FILE *fp);

/* DEBUG macros*/
#ifndef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr,\
        "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,\
        clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr,\
        "[WARN] (%s:%d: errno: %s) " M "\n",\
        __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__);\
    errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__);\
    errno=0; goto error; }

#endif /* CJCC.H */
