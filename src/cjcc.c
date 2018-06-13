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

int main(int argc, char *argv[])
{
    if(argc > 0){
        Ast *ast = scan(argv[1]);
        compile(ast);
    } else 
        error("Please give an input\n");
    return 0;
}
