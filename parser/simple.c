typedef struct MyStruct {
  int a;
  int b;
} MyStruct;

MyStruct get_struct() {
  MyStruct foo;
  foo.a = 0;
  foo.b = 0;
  return foo;
}
