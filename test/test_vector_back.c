#include "utest.h"
#include "vector.h"

struct vector_back {
    Vector *vector;
};

UTEST_F_SETUP(vector_back) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_back) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_back, null_vector_returns_null) {
    ASSERT_EQ(NULL, vector_back(NULL));
}

UTEST_F(vector_back, empty_vector_returns_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_EQ(NULL, vector_back(utest_fixture->vector));
}

UTEST_F(vector_back, accesses_last_element) {
    const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                              -49382,  0,          7483921, -1984273849,
                              429496,  12};

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    EXPECT_EQ(expected[9], *(int *)vector_back(utest_fixture->vector));
}