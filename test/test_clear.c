#include "helpers.h"
#include "utest.h"
#include "vector.h"

struct clear {
    Vector *vector;
};

UTEST_F_SETUP(clear) {
    utest_fixture->vector = NULL;
    set_up_helpers();
}

UTEST_F_TEARDOWN(clear) {
    vector_free(utest_fixture->vector);
    tear_down_helpers();
}

UTEST_F(clear, removes_all_elements) {
    utest_fixture->vector = vector_alloc(13, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);
    utest_fixture->vector->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS, vector_clear(utest_fixture->vector));
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
}

UTEST(clear, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_clear(NULL));
}

UTEST_F(clear, calls_destructor_for_every_element) {
    const int expected[13] = {33,  642, 162, 399, 643, 926, 482,
                              397, 94,  134, 648, 703, 391};
    utest_fixture->vector = vector_alloc(13, sizeof(mock_t), &mock_destroy);
    ASSERT_NE(NULL, utest_fixture->vector);

    alloc_helpers(13);

    for (size_t i = 0; i < 13; i++)
        mock_emplace_back(utest_fixture->vector, expected[i]);

    vector_clear(utest_fixture->vector);

    ASSERT_EQ((unsigned int)13, calls);
    ASSERT_MEMEQ(expected, values, sizeof(expected));
    for (size_t i = 0; i < 13; i++) {
        ASSERT_EQ((uintptr_t)MOCK_OFFSET(utest_fixture->vector->data, i),
                  addresses[i]);
    }
}