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
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);
    ASSERT_LT_MSG(utest_fixture->vector->capacity, (size_t)23,
                  "Requested capacity was pre-allocated");

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 23));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)23);
}

UTEST_F(reserve, does_nothing_when_requested_capacity_is_already_available) {
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 13));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)13);
}

UTEST(reserve, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_reserve(NULL, 13));
}

UTEST_F(reserve,
        returns_error_code_when_buffer_size_calculation_would_overflow) {
    const size_t bad_capacity = SIZE_MAX / sizeof(int) + 1;
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    ASSERT_EQ(VECTOR_LENGTH_ERROR,
              vector_reserve(utest_fixture->vector, bad_capacity));
}

UTEST_F(reserve, data_survives_buffer_growth) {
    const int expected[13] = {207, 161, 34,  542, 496, 389, 941,
                              856, 722, 424, 529, 526, 774};
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 23));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}