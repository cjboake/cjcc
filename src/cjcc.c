#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"
#include "lex.h"
#include "parse.h"
#include "util.h"
#include "gen.h"

#define EXPR_LEN 100

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
//        compile(ast);
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
