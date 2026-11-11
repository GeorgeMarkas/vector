#include "utest.h"
#include "vector.h"
#include <stddef.h>

struct vector_clear {
    Vector *vector;
};

UTEST_F_SETUP(vector_clear) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_clear) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_clear, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_clear(NULL));
}

UTEST_F(vector_clear, sets_size_to_zero) {
    ASSERT_EQ(VECTOR_SUCCESS, vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    utest_fixture->vector->size = 10;

    ASSERT_EQ(VECTOR_SUCCESS, vector_clear(utest_fixture->vector));
    EXPECT_EQ((size_t)0, utest_fixture->vector->size);
}