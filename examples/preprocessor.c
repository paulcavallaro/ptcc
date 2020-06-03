#define FUNCTION_NAME fun
#define INT int
#define CONCAT(a,b) a##b

INT FUNCTION_NAME(INT a, INT b, INT ab) {
  return CONCAT(a,b);
}
