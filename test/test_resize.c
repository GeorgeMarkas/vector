#include "utest.h"
#include "vector.h"

struct resize {
    Vector *vector;
};

UTEST_F_SETUP(resize) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(resize) {
    vector_free(utest_fixture->vector);
}

UTEST_F(resize, resizes_vector_to_given_element_count) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 6));
    ASSERT_EQ((size_t)6, utest_fixture->vector->size);
}

UTEST_F(resize, grows_buffer_when_capacity_is_not_sufficient) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 23));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)23);
}

UTEST(resize, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_resize(NULL, 13));
}

UTEST_F(resize,
        returns_error_code_when_buffer_size_calculation_would_overflow) {
    const size_t bad_count = SIZE_MAX / sizeof(int) + 1;
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    ASSERT_EQ(VECTOR_LENGTH_ERROR,
              vector_resize(utest_fixture->vector, bad_count));
}

UTEST_F(resize, data_survives_buffer_growth) {
    const int expected[13] = {1006, 207, 785, 364, 585, 475, 394,
                              617,  616, 114, 246, 91,  766};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 23));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}