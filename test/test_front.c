#include "utest.h"
#include "vector.h"

struct front {
    Vector *vector;
};

UTEST_F_SETUP(front) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(front) {
    vector_free(utest_fixture->vector);
}

UTEST_F(front, accesses_first_element) {
    const int expected[3] = {11, 22, 33};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 3;

    ASSERT_EQ(expected[0], *(int *)vector_front(utest_fixture->vector));
}

UTEST(front, returns_null_when_vector_pointer_is_null) {
    ASSERT_EQ(NULL, vector_front(NULL));
}

UTEST_F(front, returns_null_when_vector_is_empty) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 3, sizeof(int)));

    ASSERT_EQ(NULL, vector_front(utest_fixture->vector));
}