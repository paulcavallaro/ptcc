struct token {
  char a;
  int b;
};

int copy(struct token tok) {
  return tok.b;
}

int get_b(struct token* tok) {
  return tok->b;
}
