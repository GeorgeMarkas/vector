#include "helpers.h"
#include "utest.h"
#include "vector.h"

static const int expected[13] = {320, 134, 141, 25,   262, 202, 53,
                                 253, 740, 854, 1021, 518, 54};
struct erase {
    Vector *vector;
};

UTEST_F_SETUP(erase) {
    utest_fixture->vector = NULL;
    set_up_helpers();
}

UTEST_F_TEARDOWN(erase) {
    vector_free(utest_fixture->vector);
    tear_down_helpers();
}

UTEST(erase, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_erase(NULL, 0));
}

UTEST_F(erase, returns_error_code_when_pos_is_out_of_range) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));

    const size_t size = utest_fixture->vector->size;
    ASSERT_EQ(VECTOR_OUT_OF_RANGE, vector_erase(utest_fixture->vector, size));
    ASSERT_EQ(VECTOR_OUT_OF_RANGE,
              vector_erase(utest_fixture->vector, size + 1));
}

UTEST_F(erase, removes_element_at_given_pos) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_erase(utest_fixture->vector, 5));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, 5 * sizeof(int));
    ASSERT_MEMEQ(&expected[6], &((int *)utest_fixture->vector->data)[5],
                 7 * sizeof(int));
    ASSERT_EQ((size_t)12, utest_fixture->vector->size);
}

UTEST_F(erase, calls_destructor_for_element_to_be_removed) {
    ASSERT_EQ(VECTOR_SUCCESS, vector_alloc(&utest_fixture->vector, 13,
                                           sizeof(mock_t), &mock_destroy));
    alloc_helpers(13);

    for (size_t i = 0; i < 13; i++)
        mock_emplace_back(utest_fixture->vector, expected[i]);

    ASSERT_EQ(VECTOR_SUCCESS, vector_erase(utest_fixture->vector, 5));

    ASSERT_EQ((unsigned int)1, calls);
    ASSERT_EQ(expected[5], values[0]);
    ASSERT_EQ((uintptr_t)MOCK_OFFSET(utest_fixture->vector->data, 5),
              addresses[0]);
}