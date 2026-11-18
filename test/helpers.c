#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

unsigned int calls;
int *values;
uintptr_t *addresses;
size_t mock_size;

void set_up_helpers(void) {
    calls = 0;
    values = NULL;
    addresses = NULL;
    mock_size = 0;
}

void tear_down_helpers(void) {
    free(values);
    free(addresses);
    values = NULL;
    addresses = NULL;
}

void mock_emplace_back(Vector *vector, const int num) {
    assert(vector != NULL);
    assert(vector->size < vector->capacity);

    mock_t new_mock = { .num = malloc(sizeof(int)) };
    assert(new_mock.num != NULL);

    *new_mock.num = num;
    memcpy(MOCK_OFFSET(vector->data, vector->size), &new_mock, sizeof(mock_t));

    vector->size++;
    mock_size++;
}

void mock_destroy(void *element) {
    assert(calls < mock_size);
    addresses[calls] = (uintptr_t)element;
    values[calls] = *((mock_t *)element)->num;
    free(((mock_t *)element)->num);
    calls++;
}