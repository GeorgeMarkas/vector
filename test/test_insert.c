#include "utest.h"
#include "vector.h"

static const int expected[13] = {632, 207, 910, 1022, 637, 820, 634,
                                 425, 827, 744, 575,  325, 78};
struct insert {
    Vector *vector;
};

UTEST_F_SETUP(insert) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(insert) {
    vector_free(utest_fixture->vector);
}

UTEST_F(insert, inserts_at_pos_0) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, 6 * sizeof(int));
    utest_fixture->vector->size = 6;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 0, &expected[0]));
    ASSERT_EQ(expected[0], ((int *)utest_fixture->vector->data)[0]);
    ASSERT_MEMEQ(expected, &((int *)utest_fixture->vector->data)[1],
                 6 * sizeof(int));
    ASSERT_EQ((size_t)7, utest_fixture->vector->size);
}

UTEST_F(insert, inserts_at_pos_less_than_size) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, 6 * sizeof(int));
    utest_fixture->vector->size = 6;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 3, &expected[0]));
    ASSERT_EQ(expected[0], ((int *)utest_fixture->vector->data)[3]);
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, 3 * sizeof(int));
    ASSERT_MEMEQ(&expected[3], &((int *)utest_fixture->vector->data)[4],
                 3 * sizeof(int));
    ASSERT_EQ((size_t)7, utest_fixture->vector->size);
}

UTEST_F(insert, inserts_at_pos_equal_to_size) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, 6 * sizeof(int));
    utest_fixture->vector->size = 6;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 6, &expected[0]));
    ASSERT_EQ(expected[0], ((int *)utest_fixture->vector->data)[6]);
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, 6 * sizeof(int));
    ASSERT_EQ((size_t)7, utest_fixture->vector->size);
}

UTEST(insert, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_insert(NULL, 0, &expected[0]));
}

UTEST_F(insert, returns_error_code_when_value_pointer_is_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    utest_fixture->vector->size = 6;

    ASSERT_EQ(VECTOR_ILLEGAL_ARG,
              vector_insert(utest_fixture->vector, 3, NULL));
}

UTEST_F(insert, returns_error_code_when_pos_is_out_of_range) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    const size_t size = utest_fixture->vector->size;
    ASSERT_EQ(VECTOR_OUT_OF_RANGE,
              vector_insert(utest_fixture->vector, size + 1, &expected[0]));
}

UTEST_F(insert, grows_buffer_when_vector_is_full) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 13, &expected[0]));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)14);
}

UTEST_F(insert, data_survives_buffer_growth) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 13, &expected[0]));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    ASSERT_EQ(expected[0], ((int *)utest_fixture->vector->data)[13]);
}

UTEST_F(insert, handles_value_aliasing_the_buffer_when_growth_is_triggered) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 6,
                            &((int *)utest_fixture->vector->data)[9]));
    ASSERT_EQ(expected[9], ((int *)utest_fixture->vector->data)[6]);
}