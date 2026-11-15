#include "utest.h"
#include "vector.h"

struct reserve {
    Vector *vector;
};

UTEST_F_SETUP(reserve) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(reserve) {
    vector_free(utest_fixture->vector);
}

UTEST_F(reserve, reserves_requested_capacity) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    ASSERT_LT_MSG(utest_fixture->vector->capacity, (size_t)23,
                  "Requested capacity was pre-allocated");

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 23));
    ASSERT_GE((size_t)23, utest_fixture->vector->capacity);
}

UTEST_F(reserve, does_nothing_when_requested_capacity_is_already_available) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 13));
    ASSERT_GE((size_t)13, utest_fixture->vector->capacity);
}

UTEST(reserve, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_reserve(NULL, 13));
}

UTEST_F(reserve, data_survives_buffer_growth) {
    const int expected[13] = {207, 161, 34,  542, 496, 389, 941,
                              856, 722, 424, 529, 526, 774};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 23));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}