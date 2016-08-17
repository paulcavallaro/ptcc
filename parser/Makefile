CXXFLAGS = -std=c++11

all:		lexer
clean:
	rm -f lex.yy.c grammar.tab.cc grammar.tab.hh grammar.tab.c grammar.tab.h lexer *.o

lexer:		lex.yy.o grammar.tab.o
	$(CC) $(CFLAGS) -o lexer -lfl -L/usr/local/lib grammar.tab.o lex.yy.o
lex.yy.o:	lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c
lex.yy.c:	grammar.tab.cc lexer.l
	flex lexer.l
grammar.tab.cc:	grammar.y
	bison -d grammar.y
grammar.tab.o:	grammar.tab.c
	$(CC) $(CFLAGS) -c grammar.tab.c
