#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cjcc.h"


void check_file(FILE *p);
int fpeek(FILE *stream);
void error(char *input);
void expect(FILE * fp, int c);
int check_for(char c, FILE *fp);
void p(char *input);
void node_check(Ast *node, int type);

#endif
