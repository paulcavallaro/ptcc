int bits(int a, int b) {
  a = a & b;
  a = a | b;
  a = a ^ b;
  a &= b;
  a |= b;
  a ^= b;
}
