CXXFLAGS = -std=c++11

all:	jasm hello
clean:
	rm -f bin/* *.o

bindir:
	mkdir bin

jasm:	jasm.o bindir
	$(CXX) $(CXXFLAGS) jasm.o -o bin/jasm

jasm.o: jasm.cpp
	$(CXX) $(CXXFLAGS) -c jasm.cpp

hello:	hello.o bindir
	ld -s -o bin/hello hello.o

hello.o: hello_world.S
	as -64 -o bin/hello.o hello_world.S

hello_osx: hello_osx.o bindir
	ld -e _start -macosx_version_min 10.11 -arch x86_64 -lSystem -o bin/hello_osx hello_osx.o

hello_osx.o: hello_world_osx.S
	as -arch x86_64 -o hello_osx.o hello_world_osx.S
