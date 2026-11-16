#include "utest.h"
#include "vector.h"

struct erase {
    Vector *vector;
};

UTEST_F_SETUP(erase) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(erase) {
    vector_free(utest_fixture->vector);
}

UTEST(erase, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_erase(NULL, 0));
}

UTEST_F(erase, returns_error_code_when_pos_is_out_of_range) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    const size_t size = utest_fixture->vector->size;
    ASSERT_EQ(VECTOR_OUT_OF_RANGE, vector_erase(utest_fixture->vector, size));
    ASSERT_EQ(VECTOR_OUT_OF_RANGE,
              vector_erase(utest_fixture->vector, size + 1));
}

UTEST_F(erase, removes_element_at_given_pos) {
    const int expected[13] = {977, 273, 372, 650, 730, 20, 916,
                              316, 243, 691, 806, 106, 637};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_erase(utest_fixture->vector, 5));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, 5 * sizeof(int));
    ASSERT_MEMEQ(&expected[6], &((int *)utest_fixture->vector->data)[5],
                 7 * sizeof(int));
    ASSERT_EQ((size_t)12, utest_fixture->vector->size);
}