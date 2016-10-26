typedef struct Foo {
  int bar, baz;
  double doo;
  char quux;
  char *str;
  const char * ptrToConstChar;
  const char * const constPtrToConstChar;
  const char * const * ptrToConstPtrToConstChar;
  char * const * const constPtrToConstPtrToChar;
  const char * * ptrToPtrToConstChar;
  int intArr[];
} Foo;

// Simple comment

/*

Multi line comment

 */

typedef Foo Bar;

int main(int argc, char **argv) {
  Foo foo;
  foo.bar = 1;
  foo.baz = 2;
  foo.bar = 2 + 4;
  foo.doo = 1.1 + 2.2;
  foo.quux = 'c';
  foo.str = "a string on the stack";
  Bar bar;
  bar = foo;
  return 1;
}
