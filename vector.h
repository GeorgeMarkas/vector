#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t element_size;
    uint8_t *data;
    void (*element_destructor)(void *element);
} Vector;

#define VECTOR_SUCCESS 0

/* Error codes */
#define VECTOR_ILLEGAL_ARG (-1)
#define VECTOR_BAD_ALLOC (-2)
#define VECTOR_LENGTH_ERROR (-3)
#define VECTOR_OUT_OF_RANGE (-4)

/**
 * Allocate a vector that holds fixed-size elements.
 *
 * @param[out] vector             Address of the pointer that receives the new vector
 * @param      capacity           Initial element capacity
 * @param      element_size       Byte size of each element
 * @param      element_destructor Pointer to caller-defined destructor function
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_alloc(Vector **vector, const size_t capacity,
                 const size_t element_size,
                 void (*element_destructor)(void *element));

/**
 * Free all memory associated with the vector.
 *
 * @param vector Vector to free
 */
void vector_free(Vector *vector);

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
 * Checks if the vector has no elements.
 *
 * @param vector Vector to check
 * @return true if the vector is empty, false otherwise
 */
bool vector_empty(const Vector *vector);

/**
 * Reserve capacity for a minimum number of elements.
 *
 * @param vector  Vector to reserve capacity in
 * @param new_cap Element capacity to guarantee
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_reserve(Vector *vector, size_t new_cap);

/**
 * Remove unused capacity.
 *
 * @param vector Vector to trim
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_shrink_to_fit(Vector *vector);

/**
 * Removes all elements from the vector.
 *
 * @param vector Vector to clear
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_clear(Vector *vector);

/**
 * Insert an element at the specified position in the vector.
 *
 * @param vector Vector to insert into
 * @param pos    Index to insert at
 * @param value  Pointer to data
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_insert(Vector *vector, size_t pos, const void *value);

/**
 * Remove the element at the specified position in the vector.
 *
 * @param vector Vector to remove from
 * @param pos    Element index
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_erase(Vector *vector, size_t pos);

/**
 * Append an element to the end of the vector.
 *
 * @param vector Vector to append to
 * @param value  Pointer to data
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_push_back(Vector *vector, const void *value);

/**
 * Remove the last element of the vector.
 *
 * @param vector Vector to remove from
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_pop_back(Vector *vector);

/**
 * Resize the vector to contain `count` elements.
 *
 * @param vector Vector to resize
 * @param count  New size of the vector
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_resize(Vector *vector, size_t count);

/**
 * Copy vector contents into a newly allocated vector.
 *
 * @param      src  Vector to copy
 * @param[out] dest Address of the pointer that receives the vector copy
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_copy(const Vector *src, Vector **dest);