[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# vector
Dynamic array implementation in C, trying to mimick [std::vector](https://en.cppreference.com/cpp/container/vector)
API-wise. It is type-agnostic, i.e, it works with arbitrary element sizes and it supports
passing in a user-defined destructor for element types that utilize heap allocations.

## Building
You can add `vector.c` and `vector.h` to your project and use them as is.

Optionally, a static library can be created like so:
```
make release
```
*With debug symbols and sanitizers:*
```
make debug
```
This will generate `libvector.a` in `build/`. Link against it and include `vector.h`.
Example usage in project:
```
cp build/libvector.a vector.h project/vendor
```
*Resulting project structure:*
```
project
├── include
│   └── <headers>
├── src
│   └── <sources>
└── vendor
    ├── libvector.a
    └── vector.h
```
```
cd project
cc src/* -Iinclude -Ivendor -o main -Lvendor -lvector
```

Compile and run tests:
```
make test
```

## Example
*Documentation for the API functions is available in the [header](vector.h).*
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct {
    char *ptr;
    size_t size;
} String;

String string_new(const char *s) {
    String string;
    if (s == NULL) {
        string.ptr = NULL;
        string.size = 0;
    } else {
        string.size = strlen(s);
        string.ptr = strndup(s, string.size);
    }

    return string;
}

void string_destroy(void *s) {
    String string = *(String *)s;
    if (string.ptr != NULL) {
        free(string.ptr);
        string.ptr = NULL;
        string.size = 0;
    }
}

int main(void) {
    // Create an empty vector that holds `String` elements
    Vector *strings = vector_alloc(0, sizeof(String), &string_destroy);
    if (strings == NULL) {
        // Handle failure
        return -1;
    }

    String s1 = string_new("Hello, ");
    String s2 = string_new("World!\n");

    vector_push_back(strings, &s1);
    vector_push_back(strings, &s2);

    for (size_t i = 0; i < strings->size; i++) {
        String string = *(String *)vector_at(strings, i);
        fwrite(string.ptr, sizeof(char), string.size, stdout);
    }

    // Free the vector; calls string_destroy for every element
    vector_free(strings);

    return 0;
}
```