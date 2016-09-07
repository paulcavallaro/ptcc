CXXFLAGS = -std=c++14

debug:		CXXFLAGS += -DDEBUG -g
debug:		lexer
all:		lexer
clean:
	rm -f lex.yy.cpp grammar.tab.c grammar.tab.cpp grammar.tab.hpp lexer *.o flex.h

lexer:		lex.yy.o grammar.tab.o parser.o scanner.o
	$(CXX) $(CXXFLAGS) -o lexer -lfl -L/usr/local/lib grammar.tab.o lex.yy.o parser.o scanner.o
lex.yy.o:	lex.yy.cpp
	$(CXX) $(CXXFLAGS) -c lex.yy.cpp
lex.yy.cpp:	grammar.tab.cpp lexer.l
	flex -f -8 -o lex.yy.cpp lexer.l
grammar.tab.cpp:	grammar.y
	bison -d grammar.y -o grammar.tab.cpp
grammar.tab.o:	grammar.tab.cpp
	$(CXX) $(CXXFLAGS) -c grammar.tab.cpp
parser.o:	parser.cpp parser.h
	$(CXX) $(CXXFLAGS) -c parser.cpp
scanner.o:	scanner.cpp scanner.h
	$(CXX) $(CXXFLAGS) -c scanner.cpp
