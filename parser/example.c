struct Foo {
    int bar, baz;
    char quux;
};


// Simple comment

/*

Multi line comment

 */

int main(int argc, char** argv) {
    struct Foo foo;
    foo.bar = 1;
    foo.baz = 2;
    foo.quux = 'c';
    return 1;
}
