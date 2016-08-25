CXXFLAGS = -std=c++14

all:		lexer
clean:
	rm -f lex.yy.c grammar.tab.c grammar.tab.cpp grammar.tab.hpp lexer *.o

lexer:		lex.yy.o grammar.tab.o parser.o
	$(CXX) $(CXXFLAGS) -o lexer -lfl -L/usr/local/lib grammar.tab.o lex.yy.o parser.o
lex.yy.o:	lex.yy.cpp
	$(CXX) $(CXXFLAGS) -c lex.yy.cpp
lex.yy.cpp:	grammar.tab.cpp lexer.l
	flex -f -8 -o lex.yy.cpp lexer.l
grammar.tab.cpp:	grammar.y
	bison -d grammar.y -o grammar.tab.cpp
grammar.tab.o:	grammar.tab.cpp
	$(CXX) $(CXXFLAGS) -c grammar.tab.cpp
parser.o:
	$(CXX) $(CXXFLAGS) -c parser.cpp
