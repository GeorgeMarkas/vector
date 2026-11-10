#include "utest.h"
#include "vector.h"

#include <string.h>

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

UTEST_F(vector_at, out_of_range_pos_returns_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_EQ(NULL, vector_at(utest_fixture->vector, 0));
}

UTEST_F(vector_at, accesses_element_at_given_pos) {
    const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                              -49382,  0,          7483921, -1984273849,
                              429496,  12};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(expected[i], *(int *)vector_at(utest_fixture->vector, i));
    }
}