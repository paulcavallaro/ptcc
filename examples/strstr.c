const char* strstr(const char* haystack, const char* needle) {
  if (haystack == NULL) return NULL;
  if (needle == NULL) return NULL;
  while (haystack != NULL) {
    while (haystack != NULL && *haystack != *needle) {
      haystack++;
    }
    if (haystack == NULL) return NULL;
    const char* a = haystack;
    const char* b = needle;
    while (a != NULL && b != NULL && *a == *b) {
      a++;
      b++;
    }
    // Either we got a full match or we have to look forward in the haystack
    if (a == NULL && b == NULL) return haystack;
    haystack++;
  }
  return NULL;
}
