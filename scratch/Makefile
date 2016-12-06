CXXFLAGS = -std=c++11

all:	jasm hello
clean:
	rm -f jasm hello *.o

jasm:	jasm.o
	$(CXX) $(CXXFLAGS) jasm.o -o jasm

jasm.o: jasm.cpp
	$(CXX) $(CXXFLAGS) -c jasm.cpp

hello:	hello.o
	ld -s -o hello hello.o

hello.o: hello_world.S
	as -64 -o hello.o hello_world.S
