struct Foo {
    int bar, baz;
    char quux;
    char* str;
};


// Simple comment

/*

Multi line comment

 */

int main(int argc, char** argv) {
    struct Foo foo;
    foo.bar = 1;
    foo.baz = 2;
    foo.bar = 2 + 4;
    foo.quux = 'c';
    foo.str = "a string on the stack";
    return 1;
}
