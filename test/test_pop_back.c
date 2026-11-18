#include "utest.h"
#include "vector.h"

struct pop_back {
    Vector *vector;
};

UTEST_F_SETUP(pop_back) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(pop_back) {
    vector_free(utest_fixture->vector);
}

UTEST_F(pop_back, removes_last_element) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int), NULL));
    utest_fixture->vector->size = 3;

    ASSERT_EQ(VECTOR_SUCCESS, vector_pop_back(utest_fixture->vector));
    ASSERT_EQ((size_t)2, utest_fixture->vector->size);
}

UTEST_F(pop_back, does_nothing_when_vector_is_empty) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int), NULL));

    ASSERT_EQ(VECTOR_SUCCESS, vector_pop_back(utest_fixture->vector));
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
}

UTEST(pop_back, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_pop_back(NULL));
}