#include "utest.h"
#include "vector.h"

struct vector_empty {
    Vector *vector;
};

UTEST_F_SETUP(vector_empty) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_empty) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_empty, null_vector_returns_false) {
    ASSERT_FALSE(vector_empty(NULL));
}

UTEST_F(vector_empty, non_empty_vector_returns_false) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    utest_fixture->vector->size = 1;
    ASSERT_FALSE(vector_empty(utest_fixture->vector));
}

UTEST_F(vector_empty, empty_vector_returns_true) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_TRUE(vector_empty(utest_fixture->vector));
}