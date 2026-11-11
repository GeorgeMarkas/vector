#include "utest.h"
#include "vector.h"

#define GROWTH_FACTOR 2

static const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                                 -49382,  0,          7483921, -1984273849,
                                 429496,  12};

struct vector_insert {
    Vector *vector;
};

UTEST_F_SETUP(vector_insert) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_insert) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_insert, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_insert(NULL, 0, &expected[0]));
}

UTEST_F(vector_insert, null_value_returns_error) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_ILLEGAL_ARG,
              vector_insert(utest_fixture->vector, 0, NULL));
}

UTEST_F(vector_insert, out_of_range_pos_returns_error) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_OUT_OF_RANGE,
              vector_insert(utest_fixture->vector, 1, &expected[0]));
}

UTEST_F(vector_insert, inserts_at_pos_zero) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, &expected[1], 9 * sizeof(int));
    utest_fixture->vector->size = 9;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 0, &expected[0]));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    EXPECT_EQ((size_t)10, utest_fixture->vector->size);
}

UTEST_F(vector_insert, inserts_at_pos_less_than_size) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, 4 * sizeof(int));
    memcpy(&((int *)utest_fixture->vector->data)[4], &expected[5],
           5 * sizeof(int));
    utest_fixture->vector->size = 9;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 4, &expected[4]));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    EXPECT_EQ((size_t)10, utest_fixture->vector->size);
}

UTEST_F(vector_insert, inserts_at_pos_equal_to_size) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, 9 * sizeof(int));
    utest_fixture->vector->size = 9;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 9, &expected[9]));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    EXPECT_EQ((size_t)10, utest_fixture->vector->size);
}

// This also implicitly tests if data survives growth
UTEST_F(vector_insert, handles_value_aliasing_the_buffer) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    // 'value' points to the buffer half about to be shifted
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_insert(utest_fixture->vector, 4,
                            &((int *)utest_fixture->vector->data)[6]));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, 4 * sizeof(int));
    EXPECT_EQ(expected[6], ((int *)utest_fixture->vector->data)[4]);
    EXPECT_MEMEQ(&expected[4], &((int *)utest_fixture->vector->data)[5],
                 6 * sizeof(int));
    EXPECT_EQ((size_t)11, utest_fixture->vector->size);
    EXPECT_EQ((size_t)10 * GROWTH_FACTOR, utest_fixture->vector->capacity);
}