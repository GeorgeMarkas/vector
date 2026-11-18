#pragma once

#include "vector.h"

#define MOCK_OFFSET(base, pos) ((base) + (pos) * sizeof(mock_t))

typedef struct Mock_Type {
    int *num;
} mock_t;

extern unsigned int calls;   // How many times the destructor was called
extern int *values;          // Freed element data
extern uintptr_t *addresses; // Element addresses the destructor was called for
extern size_t mock_size;     // Vector size available to the helper functions

void set_up_helpers(void);

void tear_down_helpers(void);

void mock_emplace_back(Vector *vector, const int num);

void mock_destroy(void *mock);
