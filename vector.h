#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t element_size;
    uint8_t *data;
    // TODO: Optional function pointer to element destructor
} Vector;

// TODO: Flesh our error codes
#define VECTOR_SUCCESS 0
#define VECTOR_ERROR (-1)

/**
 * Convenience macro that defines typed accessors for a T vector.
 * Each accessor is a function wrapper for its untyped counterpart,
 * casting the returned pointer to T * and returning the dereferenced
 * value.
 *
 * T must be a single identifier e.g., 'uint32_t', not 'unsigned int'.
 */
#define VECTOR_DEFINE_TYPED_ACCESSORS(T)                                       \
    static inline T vector_##T##_at(const Vector *vector, size_t pos) {        \
        return *(T *)vector_at(vector, pos);                                   \
    }                                                                          \
                                                                               \
    static inline T vector_##T##_front(const Vector *vector) {                 \
        return *(T *)vector_front(vector);                                     \
    }                                                                          \
                                                                               \
    static inline T vector_##T##_back(const Vector *vector) {                  \
        return *(T *)vector_back(vector);                                      \
    }

/**
 * Allocate a vector that holds fixed-size elements.
 *
 * @param capacity     Initial element capacity
 * @param element_size Byte size of each element
 * @return Pointer to the newly-allocated vector or NULL on error
 */
Vector *vector_alloc(size_t capacity, size_t element_size);

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
 * @param vector   Vector to reserve capacity in
 * @param capacity Element capacity to guarantee
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_reserve(Vector *vector, size_t capacity);

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
 * Resize the vector to contain new_size elements.
 *
 * @param vector   Vector to resize
 * @param new_size New size of the vector
 * @return VECTOR_SUCCESS on success or an error code on failure
 */
int vector_resize(Vector *vector, size_t new_size);

/**
 * Copy vector contents into a newly allocated vector.
 *
 * @param vector Vector to copy
 * @return Pointer to the newly allocated vector or NULL on error
 */
Vector *vector_copy(const Vector *vector);