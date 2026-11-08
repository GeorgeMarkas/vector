#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t element_size;
    uint8_t *data;
    // TODO: Optional function pointer to element destructor
} Vector;

#define VECTOR_SUCCESS 0
#define VECTOR_ERROR (-1)

/**
 * Allocate a vector that holds fixed-size elements.
 *
 * @param capacity     Initial element capacity
 * @param element_size Element byte size
 * @return Pointer to the newly-allocated vector or NULL on error
 */
Vector *vector_alloc(size_t capacity, size_t element_size);

/**
 * Copy vector contents into a newly-allocated vector.
 *
 * @param vector Vector to copy
 * @return Pointer to the newly-allocated vector or NULL on error
 */
Vector *vector_copy(const Vector *vector);

/**
 * Allocate additional vector capacity.
 *
 * @param vector   Vector to allocate for
 * @param capacity Element capacity to allocate
 * @return Error code (`VECTOR_SUCCESS` if successful)
 */
int vector_reserve(Vector *vector, size_t capacity);

/**
 * Append an element to the end of the vector.
 *
 * @param vector Vector to append to
 * @param value  Element value
 * @return Error code (`VECTOR_SUCCESS` if successful)
 */
int vector_push_back(Vector *vector, const void *value);

/**
 * Insert an element at the specified position in the vector.
 *
 * @param vector Vector to insert into
 * @param pos    Position to insert at
 * @param value  Element value
 * @return Error code (`VECTOR_SUCCESS` if successful)
 */
int vector_insert(Vector *vector, size_t pos, const void *value);

/**
 * Free all memory associated with the vector.
 *
 * @param vector Vector to free
 */
void vector_free(Vector *vector);
