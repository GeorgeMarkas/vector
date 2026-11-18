#include "utest.h"
#include "vector.h"

struct shrink_to_fit {
    Vector *vector;
};

UTEST_F_SETUP(shrink_to_fit) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(shrink_to_fit) {
    vector_free(utest_fixture->vector);
}

UTEST_F(shrink_to_fit, removes_unused_capacity) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 23, sizeof(int), NULL));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_shrink_to_fit(utest_fixture->vector));
    ASSERT_EQ((size_t)13, utest_fixture->vector->capacity);
}

UTEST(shrink_to_fit, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_shrink_to_fit(NULL));
}

UTEST_F(shrink_to_fit, data_survives_buffer_shrinkage) {
    const int expected[13] = {295,  125, 275, 433, 492, 898, 419,
                              1005, 705, 585, 802, 201, 537};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 23, sizeof(int), NULL));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_shrink_to_fit(utest_fixture->vector));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}