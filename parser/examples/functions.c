int mult(int left, int right) {
  return left * right;
}

float fmuls(float left, float right) {
  return left * right;
}

double fmuld(double left, double right) {
  return left * right;
}

int add(int a, int b) {
  return a + b;
}

// This is the stupidest fucking syntax ever, and I don't think any compiler
// even supports the semantics in the spec.
int sum(a, b)
int a, b; {
  return a + b;
}

int call_sum(float a, float b) {
  return sum(a, b);
}

int call_add(float a, float b) {
  return add(a, b);
}

unsigned int gcd(unsigned int a, unsigned int b) {
  unsigned int tmp;
  while (b != 0) {
    tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

