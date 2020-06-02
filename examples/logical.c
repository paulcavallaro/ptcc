bool logical(int a, int b) {
  if ((a ? b : a) && a || b) return true;
  return false;
}
