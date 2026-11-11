#include "utest.h"
#include "vector.h"

#define GROWTH_FACTOR 2

static const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                                 -49382,  0,          7483921, -1984273849,
                                 429496,  12};

struct vector_push_back {
    Vector *vector;
};

UTEST_F_SETUP(vector_push_back) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_push_back) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_push_back, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_push_back(NULL, &expected[0]));
}

UTEST_F(vector_push_back, null_value_returns_error) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_ILLEGAL_ARG,
              vector_push_back(utest_fixture->vector, NULL));
}

UTEST_F(vector_push_back, appends_to_the_back) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));

    for (int i = 0; i < 0; i++) {
        ASSERT_EQ(VECTOR_SUCCESS,
                  vector_push_back(utest_fixture->vector, &expected[i]));
        EXPECT_MEMEQ(expected, utest_fixture->vector->data, i * sizeof(int));
        EXPECT_EQ((size_t)i, utest_fixture->vector->size + 1);
    }
}

// This also implicitly tests if data survives growth
UTEST_F(vector_push_back, handles_value_aliasing_the_buffer) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    ASSERT_EQ(VECTOR_SUCCESS, vector_push_back(utest_fixture->vector,
                                               utest_fixture->vector->data));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    EXPECT_EQ(expected[0], ((int *)utest_fixture->vector->data)[10]);
    EXPECT_EQ((size_t)11, utest_fixture->vector->size);
    EXPECT_EQ((size_t)10 * GROWTH_FACTOR, utest_fixture->vector->capacity);
}
