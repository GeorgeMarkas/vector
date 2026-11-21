#include "helpers.h"
#include "utest.h"
#include "vector.h"

static const int expected[13] = {693, 430, 503, 673, 0,  108, 321,
                                 47,  360, 93,  255, 18, 269};
struct resize {
    Vector *vector;
};

UTEST_F_SETUP(resize) {
    utest_fixture->vector = NULL;
    set_up_helpers();
}

UTEST_F_TEARDOWN(resize) {
    vector_free(utest_fixture->vector);
    tear_down_helpers();
}

UTEST_F(resize, resizes_vector_to_given_element_count) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 6));
    ASSERT_EQ((size_t)6, utest_fixture->vector->size);
}

UTEST_F(resize, grows_buffer_when_capacity_is_not_sufficient) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 23));
    ASSERT_GE(utest_fixture->vector->capacity, (size_t)23);
}

UTEST(resize, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_resize(NULL, 13));
}

UTEST_F(resize,
        returns_error_code_when_buffer_size_calculation_would_overflow) {
    const size_t bad_count = SIZE_MAX / sizeof(int) + 1;
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));

    ASSERT_EQ(VECTOR_LENGTH_ERROR,
              vector_resize(utest_fixture->vector, bad_count));
}

UTEST_F(resize, data_survives_buffer_growth) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int), NULL));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 23));
    ASSERT_MEMEQ(expected, utest_fixture->vector->data, sizeof(expected));
}

UTEST_F(resize, calls_destructor_for_elements_to_be_removed) {
    ASSERT_EQ(VECTOR_SUCCESS, vector_alloc(&utest_fixture->vector, 13,
                                           sizeof(mock_t), &mock_destroy));
    alloc_helpers(13);

    for (size_t i = 0; i < 13; i++)
        mock_emplace_back(utest_fixture->vector, expected[i]);

    ASSERT_EQ(VECTOR_SUCCESS, vector_resize(utest_fixture->vector, 6));

    ASSERT_EQ((unsigned int)7, calls);
    ASSERT_MEMEQ(&expected[6], values, 7 * sizeof(int));
    for (size_t i = 6; i < 13; i++) {
        ASSERT_EQ((uintptr_t)MOCK_OFFSET(utest_fixture->vector->data, i),
                  addresses[i - 6]);
    }
}