CXXFLAGS = -std=c++11

all:	jasm
clean:
	rm -f jasm

jasm:	jasm.o
	$(CXX) $(CXXFLAGS) jasm.o -o jasm

jasm.o: jasm.cpp
	$(CXX) $(CXXFLAGS) -c jasm.cpp
