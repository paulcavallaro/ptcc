int hex() {
  return 0x01234 + 0x56789 + 0xabcdef +0xABCDEF + 0x77ff0 + 0x77FF0;
}

int octal() {
  return 077;
}

int decimal() {
  return 123;
}

float floaty() {
  return (123e2 +
          123e+2 +
          123e-2 +
          1.23e2 +
          1.23e+2 +
          1.23e-2 +
          123E2 +
          123E+2 +
          123E-2 +
          1.23E2 +
          1.23E+2 +
          1.23E-3 +
          .123 +
          0.123 +
          012.34e-1 +
          .123e1 +
          0x.aap2 +
          0xFFp-1 +
          0xffp-1 +
          0xFFP-1 +
          0xffP-1 +
          0x1a.p+2);
}
