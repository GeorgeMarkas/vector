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
 * @param element_size Byte size of each element
 * @return Pointer to the newly-allocated vector or NULL on error
 */
Vector *vector_alloc(size_t capacity, size_t element_size);

/**
 * Copy vector contents into a newly allocated vector.
 *
 * @param vector Vector to copy
 * @return Pointer to the newly allocated vector or NULL on error
 */
Vector *vector_copy(const Vector *vector);

/**
 * Reserve capacity for a minimum number of elements.
 *
 * @param vector   Vector to reserve capacity in
 * @param capacity Element capacity to guarantee
 * @return `VECTOR_SUCCESS` on success or an error code on failure
 */
int vector_reserve(Vector *vector, size_t capacity);

/**
 * Append an element to the end of the vector.
 *
 * @param vector Vector to append to
 * @param value  Pointer to data
 * @return `VECTOR_SUCCESS` on success or an error code on failure
 */
int vector_push_back(Vector *vector, const void *value);

/**
 * Insert an element at the specified position in the vector.
 *
 * @param vector Vector to insert into
 * @param pos    Index to insert at
 * @param value  Pointer to data
 * @return `VECTOR_SUCCESS` on success or an error code on failure
 */
int vector_insert(Vector *vector, size_t pos, const void *value);

/**
 * Get a pointer to the element at the specified position in the vector.
 *
 * @param vector Vector to access
 * @param pos    Element index
 * @return Pointer to the element or NULL on error
 */
void *vector_at(const Vector *vector, size_t pos);

/**
 * Get a pointer to the first element of the vector.
 *
 * @param vector Vector to access
 * @return Pointer to the element or NULL on error
 */
void *vector_front(const Vector *vector);

/**
 * Get a pointer to the last element of the vector.
 *
 * @param vector Vector to access
 * @return Pointer to the element or NULL on error
 */
void *vector_back(const Vector *vector);

/**
 * Resize the vector to contain `new_size` elements.
 *
 * @param vector   Vector to resize
 * @param new_size New size of the vector
 * @return `VECTOR_SUCCESS` on success or an error code on failure
 */
int vector_resize(Vector *vector, size_t new_size);

/**
 * Remove unused capacity.
 * 
 * @param vector Vector to trim
 * @return `VECTOR_SUCCESS` on success or an error code on failure
 */
int vector_shrink_to_fit(Vector *vector);

/**
 * Free all memory associated with the vector.
 *
 * @param vector Vector to free
 */
void vector_free(Vector *vector);
