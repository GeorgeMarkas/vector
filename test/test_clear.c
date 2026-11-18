#include "utest.h"
#include "vector.h"

struct clear {
    Vector *vector;
};

UTEST_F_SETUP(clear) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(clear) {
    vector_free(utest_fixture->vector);
}

UTEST_F(clear, removes_all_elements) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_clear(utest_fixture->vector));
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
}

UTEST(clear, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_clear(NULL));
}