char* hello() {
  return "hello";
}

char* utf8_literal() {
  return u8"☃";
}

char* wide_literal() {
  return U"Wide";
}

char* wilde_literal() {
  return u"Wilde";
}

char* oscar_wilde_literal() {
  return L"Oscar Wilde";
}

char* escaped_string() {
  return "\t\tA man\n\t\tA plan\n\t\tA canal\n\t\tPanama";
}
