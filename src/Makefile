## Makefile for the cjcc compiler
CFLAGS=-Wall -g
OBJS=cjcc.o lex.o parse.o util.o 

cjcc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f cjcc *.o tmp.*
