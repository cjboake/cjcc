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

List *scan(char *input)
{
    FILE *fp;
    Ast *ast;
    fp = fopen(input, "r");
    check_file(fp);
    List *block = make_list();
    for(;;){    
        ast = read_expr(fp);
        if(ast) list_append(block, ast);
        if(!ast) break;
    }
    return block;
}

int main(int argc, char *argv[])
{
    if(argc > 0){
        List *program = scan(argv[1]);
        compile(program);
    } else 
        error("Please give an input\n");
    return 0;
}
