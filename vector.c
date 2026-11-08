#include <stdlib.h>
#include <string.h>

#include "vector.h"

#define GROWTH_FACTOR 2
#define TEMP_BUFSIZE 64

#define ELEM_BYTES(element_count) ((element_count) * vector->element_size)
#define ELEM_OFFSET(base, pos) ((base) + ELEM_BYTES(pos))
#define SIZE_OFFSET(base) ((base) + ELEM_BYTES(vector->size))

static int adjust_capacity(Vector *vector, size_t new_capacity) {
    if (new_capacity > (SIZE_MAX / vector->element_size)) return VECTOR_ERROR;

    uint8_t *resized_buffer = realloc(vector->data, ELEM_BYTES(new_capacity));
    if (resized_buffer == NULL) return VECTOR_ERROR;
    vector->data = resized_buffer;
    vector->capacity = new_capacity;

    return VECTOR_SUCCESS;
}

static uint8_t *alloc_new_buffer(const size_t capacity,
                                 const size_t element_size,
                                 size_t *new_capacity) {
    if (capacity == 0) {
        *new_capacity = 1;
    } else {
        if (capacity > (SIZE_MAX / GROWTH_FACTOR)) return NULL;
        *new_capacity = capacity * GROWTH_FACTOR;
    }
    if (*new_capacity > (SIZE_MAX / element_size)) return NULL;

    return malloc(*new_capacity * element_size);
}

Vector *vector_alloc(const size_t capacity, const size_t element_size) {
    if (element_size < 1) return NULL;
    if (capacity > (SIZE_MAX / element_size)) return NULL;

    Vector *vector = malloc(sizeof(Vector));
    if (vector == NULL) return NULL;

    if (capacity > 0) {
        vector->data = malloc(capacity * element_size);
        if (vector->data == NULL) {
            free(vector);
            return NULL;
        }
    } else {
        vector->data = NULL;
    }

    vector->size = 0;
    vector->capacity = capacity;
    vector->element_size = element_size;

    return vector;
}

void vector_free(Vector *vector) {
    if (vector == NULL) return;
    free(vector->data);
    free(vector);
}

void *vector_at(const Vector *vector, size_t pos) {
    if (vector == NULL || pos >= vector->size) return NULL;

    return ELEM_OFFSET(vector->data, pos);
}

void *vector_front(const Vector *vector) {
    if (vector == NULL || vector->size == 0) return NULL;

    return ELEM_OFFSET(vector->data, 0);
}

void *vector_back(const Vector *vector) {
    if (vector == NULL || vector->size == 0) return NULL;

    return ELEM_OFFSET(vector->data, vector->size - 1);
}

bool vector_empty(const Vector *vector) {
    if (vector == NULL) return false;

    return vector->size == 0;
}

int vector_reserve(Vector *vector, const size_t capacity) {
    if (vector == NULL) return VECTOR_ERROR;
    if (capacity <= vector->capacity) return VECTOR_SUCCESS;

    return adjust_capacity(vector, capacity);
}

int vector_shrink_to_fit(Vector *vector) {
    if (vector == NULL) return VECTOR_ERROR;

    return adjust_capacity(vector, vector->size);
}

int vector_clear(Vector *vector) {
    if (vector == NULL) return NULL;
    vector->size = 0;

    return VECTOR_SUCCESS;
}

int vector_insert(Vector *vector, const size_t pos, const void *value) {
    if (vector == NULL || pos > vector->size || value == NULL)
        return VECTOR_ERROR;

    if (vector->size == vector->capacity) {
        size_t new_capacity;
        uint8_t *new_buffer = alloc_new_buffer(vector->capacity,
                                               vector->element_size,
                                               &new_capacity);
        if (new_buffer == NULL) return VECTOR_ERROR;

        if (pos > 0) memcpy(new_buffer, vector->data, ELEM_BYTES(pos));

        memcpy(ELEM_OFFSET(new_buffer, pos), value, vector->element_size);

        if (pos < vector->size)
            memcpy(ELEM_OFFSET(new_buffer, pos + 1),
                   ELEM_OFFSET(vector->data, pos),
                   ELEM_BYTES(vector->size - pos));

        free(vector->data);
        vector->data = new_buffer;
        vector->capacity = new_capacity;
    } else {
        uint8_t temp_buffer[TEMP_BUFSIZE];
        uint8_t *temp = temp_buffer;
        if (vector->element_size > sizeof(temp_buffer)) {
            temp = malloc(vector->element_size);
            if (temp == NULL) return VECTOR_ERROR;
        }

        // Copy the data pointed to by `value` to a temporary buffer
        // before calling memmove, in case `value` points within
        // the vector.
        memcpy(temp, value, vector->element_size);

        memmove(ELEM_OFFSET(vector->data, pos + 1),
                ELEM_OFFSET(vector->data, pos), ELEM_BYTES(vector->size - pos));

        memcpy(ELEM_OFFSET(vector->data, pos), temp, vector->element_size);

        if (temp != temp_buffer) free(temp);
    }

    vector->size++;

    return VECTOR_SUCCESS;
}

int vector_erase(Vector *vector, const size_t pos) {
    // TODO: Implement
}

int vector_push_back(Vector *vector, const void *value) {
    if (vector == NULL || value == NULL) return VECTOR_ERROR;

    // Size being equal to capacity means that either the
    // vector is full or that it is empty with 0 capacity.
    if (vector->size == vector->capacity) {
        size_t new_capacity;
        uint8_t *new_buffer = alloc_new_buffer(vector->capacity,
                                               vector->element_size,
                                               &new_capacity);
        if (new_buffer == NULL) return VECTOR_ERROR;

        memcpy(SIZE_OFFSET(new_buffer), value, vector->element_size);

        if (vector->size > 0)
            memcpy(new_buffer, vector->data, ELEM_BYTES(vector->size));

        free(vector->data);
        vector->data = new_buffer;
        vector->capacity = new_capacity;
    } else {
        memcpy(SIZE_OFFSET(vector->data), value, vector->element_size);
    }

    vector->size++;

    return VECTOR_SUCCESS;
}

int vector_pop_back(Vector *vector) {
    // TODO: Implement
}

int vector_resize(Vector *vector, const size_t new_size) {
    if (vector == NULL) return VECTOR_ERROR;

    if (new_size > vector->capacity &&
        adjust_capacity(vector, new_size) != VECTOR_SUCCESS)
        return VECTOR_ERROR;

    if (new_size > vector->size)
        memset(SIZE_OFFSET(vector->data), 0,
               ELEM_BYTES(new_size - vector->size));

    vector->size = new_size;

    return VECTOR_SUCCESS;
}

Vector *vector_copy(const Vector *vector) {
    if (vector == NULL) return NULL;

    Vector *copy = malloc(sizeof(Vector));
    if (copy == NULL) return NULL;

    if (vector->capacity > 0) {
        copy->data = malloc(vector->capacity * vector->element_size);
        if (copy->data == NULL) {
            free(copy);
            return NULL;
        }

        if (vector->size > 0)
            memcpy(copy->data, vector->data, ELEM_BYTES(vector->size));

    } else {
        copy->data = NULL;
    }

    copy->size = vector->size;
    copy->capacity = vector->capacity;
    copy->element_size = vector->element_size;

    return copy;
}