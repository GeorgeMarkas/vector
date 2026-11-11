#include "utest.h"
#include "vector.h"

struct vector_erase {
    Vector *vector;
};

UTEST_F_SETUP(vector_erase) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_erase) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_erase, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_erase(NULL, 0));
}

UTEST_F(vector_erase, out_of_range_pos_returns_error) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_OUT_OF_RANGE, vector_erase(utest_fixture->vector, 0));
    ASSERT_EQ(VECTOR_OUT_OF_RANGE, vector_erase(utest_fixture->vector, 1));
}

UTEST_F(vector_erase, removes_element_at_given_pos) {
    const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                              -49382,  0,          7483921, -1984273849,
                              429496,  12};

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    ASSERT_EQ(VECTOR_SUCCESS, vector_erase(utest_fixture->vector, 4));
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, 4 * sizeof(int));
    EXPECT_MEMEQ(&expected[5], &((int *)utest_fixture->vector->data)[4],
                 5 * sizeof(int));
    EXPECT_EQ((size_t)9, utest_fixture->vector->size);
}