#include <stdlib.h>

struct MyStruct {
  int a;
  int b;
};

struct MyStruct* get_struct() {
  struct MyStruct *foo = malloc(sizeof(struct MyStruct));
  if (foo == NULL) {
    return NULL;
  }
  foo->a = 0;
  foo->b = 0;
  return foo;
}
