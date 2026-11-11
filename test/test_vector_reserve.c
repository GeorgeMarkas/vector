#include "utest.h"
#include "vector.h"

static const int expected[10] = {8439201, -153029482, 93847,   2048593821,
                                 -49382,  0,          7483921, -1984273849,
                                 429496,  12};

struct vector_reserve {
    Vector *vector;
};

UTEST_F_SETUP(vector_reserve) {
    utest_fixture->vector = NULL;
    (void)utest_result;
}

UTEST_F_TEARDOWN(vector_reserve) {
    vector_free(utest_fixture->vector);
    (void)utest_result;
}

UTEST(vector_reserve, null_vector_returns_error) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_reserve(NULL, 10));
}

// Technically, the following 2 tests do not check if the same capacity
// value gets re-assigned to the vector or if the allocated buffer gets
// altered, but that's being pedantic because the actual implementation
// for vector_reserve is quite simple.
UTEST_F(vector_reserve, capacity_less_than_current_does_nothing) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 9));
    EXPECT_EQ(utest_fixture->vector->capacity, (size_t)10);
}

UTEST_F(vector_reserve, capacity_equal_to_current_does_nothing) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));
    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 10));
    EXPECT_EQ(utest_fixture->vector->capacity, (size_t)10);
}

UTEST_F(vector_reserve, reserves_writable_capacity) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 0, sizeof(int)));
    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 10));
    EXPECT_EQ((size_t)10, utest_fixture->vector->capacity);

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
    // Hopefully, if something isn't right by this point, the address sanitizer
    // will have screamed.
}

UTEST_F(vector_reserve, data_survives_buffer_growth) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 10, sizeof(int)));

    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 10;

    ASSERT_EQ(VECTOR_SUCCESS, vector_reserve(utest_fixture->vector, 20));
    ASSERT_EQ((size_t)20, utest_fixture->vector->capacity);
    EXPECT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}
