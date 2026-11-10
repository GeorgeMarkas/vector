#include "utest.h"
#include "vector.h"

#include <stddef.h>

struct vector_alloc {
    Vector *vector;
};

UTEST_F_SETUP(vector_alloc) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_alloc) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_alloc, null_vector_pointer_address_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_alloc(NULL, 10, sizeof(int)));
}

UTEST_F(vector_alloc, zero_element_size_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_alloc(&utest_fixture->vector, 10, 0));
}

UTEST_F(vector_alloc, capacity_overflow_returns_error) {
    const size_t capacity = SIZE_MAX / sizeof(int) + 1;
    ASSERT_EQ(VECTOR_LENGTH_ERROR,
              vector_alloc(&utest_fixture->vector, capacity, sizeof(int)));
}

UTEST_F(vector_alloc, non_zero_capacity_succeeds) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_NE(NULL, utest_fixture->vector);
    ASSERT_EQ(NULL, utest_fixture->vector->data);
    EXPECT_EQ((size_t)0, utest_fixture->vector->size);
    EXPECT_EQ((size_t)0, utest_fixture->vector->capacity);
    EXPECT_EQ(sizeof(int), utest_fixture->vector->element_size);
}

UTEST_F(vector_alloc, zero_capacity_succeeds) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_NE(NULL, utest_fixture->vector);
    ASSERT_NE(NULL, utest_fixture->vector->data);
    EXPECT_EQ((size_t)0, utest_fixture->vector->size);
    EXPECT_EQ((size_t)10, utest_fixture->vector->capacity);
    EXPECT_EQ(sizeof(int), utest_fixture->vector->element_size);
}
