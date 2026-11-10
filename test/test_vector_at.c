#include "utest.h"

#include "vector.h"

struct vector_at {
    Vector *vector;
};

UTEST_F_SETUP(vector_at) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_at) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_at, null_vector_returns_null) {
    ASSERT_EQ(NULL, vector_at(NULL, 0));
}

UTEST_F(vector_at, empty_vector_returns_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_EQ(NULL, vector_at(utest_fixture->vector, 0));
}