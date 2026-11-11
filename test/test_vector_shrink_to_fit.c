#include "utest.h"
#include "vector.h"

struct vector_shrink_to_fit {
    Vector *vector;
};

UTEST_F_SETUP(vector_shrink_to_fit) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_shrink_to_fit) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_shrink_to_fit, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_shrink_to_fit(NULL));
}

UTEST_F(vector_shrink_to_fit, removes_unused_capacity) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_SUCCESS, vector_shrink_to_fit(utest_fixture->vector));
    // This implicitly tests the 'new_capacity == 0' branch of adjust_capacity
    EXPECT_EQ((size_t)0, utest_fixture->vector->size);
    EXPECT_EQ((size_t)0, utest_fixture->vector->capacity);
    EXPECT_EQ(NULL, utest_fixture->vector->data);
}

UTEST_F(vector_shrink_to_fit, does_not_mess_with_live_data) {
    const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                              -49382,  0,          7483921, -1984273849,
                              429496,  12};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 20, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    ASSERT_EQ(VECTOR_SUCCESS, vector_shrink_to_fit(utest_fixture->vector));
    EXPECT_EQ((size_t)10, utest_fixture->vector->capacity);
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}