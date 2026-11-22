#include "utest.h"
#include "vector.h"

struct back {
    Vector *vector;
};

UTEST_F_SETUP(back) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(back) {
    vector_free(utest_fixture->vector);
}

UTEST_F(back, accesses_last_element) {
    const int expected[3] = {11, 22, 33};
    utest_fixture->vector = vector_alloc(3, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 3;

    ASSERT_EQ(expected[2], *(int *)vector_back(utest_fixture->vector));
}

UTEST(back, returns_null_when_vector_pointer_is_null) {
    ASSERT_EQ(NULL, vector_back(NULL));
}

UTEST_F(back, returns_null_when_vector_is_empty) {
    utest_fixture->vector = vector_alloc(3, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    ASSERT_EQ(NULL, vector_back(utest_fixture->vector));
}