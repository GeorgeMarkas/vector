#include "utest.h"
#include "vector.h"

struct alloc {
    Vector *vector;
};

UTEST_F_SETUP(alloc) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(alloc) {
    vector_free(utest_fixture->vector);
}

UTEST_F(alloc, allocates_vector_with_requested_capacity) {
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);

    ASSERT_NE(NULL, utest_fixture->vector);
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)13);
    ASSERT_EQ(sizeof(int), utest_fixture->vector->element_size);
    ASSERT_NE(NULL, utest_fixture->vector->data);
    ASSERT_TRUE(NULL == utest_fixture->vector->element_destructor);
}

UTEST_F(alloc, allocates_vector_with_no_buffer_when_requested_capacity_is_0) {
    utest_fixture->vector = vector_alloc(0, sizeof(int), NULL);

    ASSERT_NE(NULL, utest_fixture->vector);
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
    ASSERT_EQ((size_t)0, utest_fixture->vector->capacity);
    ASSERT_EQ(sizeof(int), utest_fixture->vector->element_size);
    ASSERT_EQ(NULL, utest_fixture->vector->data);
    ASSERT_TRUE(NULL == utest_fixture->vector->element_destructor);
}

UTEST_F(alloc, returns_null_when_element_size_is_0) {
    utest_fixture->vector = vector_alloc(13, 0, NULL);
    ASSERT_EQ(NULL, utest_fixture->vector);
}

UTEST_F(alloc, returns_null_when_buffer_size_calculation_would_overflow) {
    const size_t bad_capacity = SIZE_MAX / sizeof(int) + 1;
    utest_fixture->vector = vector_alloc(bad_capacity, sizeof(int), NULL);
    ASSERT_EQ(NULL, utest_fixture->vector);
}