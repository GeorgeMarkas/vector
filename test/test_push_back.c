#include "utest.h"
#include "vector.h"

static const int expected[13] = {220,  529, 532, 292, 335, 191, 291,
                                 1020, 906, 384, 166, 555, 851};
struct push_back {
    Vector *vector;
};

UTEST_F_SETUP(push_back) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(push_back) {
    vector_free(utest_fixture->vector);
}

UTEST_F(push_back, appends_to_the_end) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    for (size_t i = 0; i < 13; i++) {
        ASSERT_EQ(VECTOR_SUCCESS,
                  vector_push_back(utest_fixture->vector, &expected[i]));
        ASSERT_MEMEQ(expected, utest_fixture->vector->data, i * sizeof(int));
        ASSERT_EQ(i + 1, utest_fixture->vector->size);
    }
}

UTEST(push_back, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_push_back(NULL, &expected[0]));
}

UTEST_F(push_back, returns_error_code_when_value_pointer_is_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    utest_fixture->vector->size = 6;

    ASSERT_EQ(VECTOR_ILLEGAL_ARG,
              vector_push_back(utest_fixture->vector, NULL));
}

UTEST_F(push_back, triggers_buffer_growth_when_vector_is_full) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_push_back(utest_fixture->vector, &expected[0]));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)14);
}

UTEST_F(push_back, data_survives_buffer_growth) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_push_back(utest_fixture->vector, &expected[0]));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    ASSERT_EQ(expected[0], ((int *)utest_fixture->vector->data)[13]);
}

UTEST_F(push_back, handles_value_aliasing_the_buffer_when_growth_is_triggered) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_push_back(utest_fixture->vector,
                               &((int *)utest_fixture->vector->data)[9]));
    ASSERT_EQ(expected[9], ((int *)utest_fixture->vector->data)[13]);
}