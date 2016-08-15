CXXFLAGS = -std=c++11

all:		lexer
clean:
	rm -f lex.yy.cc lex.yy.c grammar.tab.cc grammar.tab.hh lexer

lexer:		lex.yy.c grammar.tab.cc
	$(CXX) $(CXXFLAGS) -o lexer grammar.tab.cc lex.yy.c
lex.yy.c:
	flex lexer.ll
grammar.tab.cc:
	bison -d grammar.yy
