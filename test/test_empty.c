#include "utest.h"
#include "vector.h"

struct empty {
    Vector *vector;
};

UTEST_F_SETUP(empty) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(empty) {
    vector_free(utest_fixture->vector);
}

UTEST_F(empty, returns_true_when_vector_is_empty) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int)));

    ASSERT_TRUE(vector_empty(utest_fixture->vector));
}

UTEST_F(empty, returns_false_when_vector_is_not_empty) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int)));
    utest_fixture->vector->size = 3;

    ASSERT_FALSE(vector_empty(utest_fixture->vector));
}

UTEST(empty, returns_false_when_vector_pointer_is_null) {
    ASSERT_FALSE(vector_empty(NULL));
}