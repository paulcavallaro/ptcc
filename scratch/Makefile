CXXFLAGS = -std=c++11

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	CC = clang
	LDFLAGS += -mmacosx_version_min=10.10
endif

all:	jasm hello
clean:
	rm -f bin/* *.o

bindir:
	mkdir -p bin

jasm:	jasm.o bindir
	$(CXX) $(CXXFLAGS) jasm.o -o bin/jasm

jasm.o: jasm.cpp
	$(CXX) $(CXXFLAGS) -c jasm.cpp

hello:	hello_world.o bindir
	$(CC) $(CFLAGS) -static -o bin/hello hello_world.o

hello_world.o: hello_world.S
	$(CC) $(CFLAGS) -fPIC -c hello_world.S

hello_osx:	hello_world_osx.o bindir
	ld -e main -macosx_version_min 10.11 -arch x86_64 -lSystem -o bin/hello_osx hello_world_osx.o

hello_world_osx.o:	hello_world_osx.S
	$(CC) -fPIC -c hello_world_osx.S
