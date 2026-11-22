#include "helpers.h"
#include "utest.h"
#include "vector.h"

struct pop_back {
    Vector *vector;
};

UTEST_F_SETUP(pop_back) {
    utest_fixture->vector = NULL;
    set_up_helpers();
}

UTEST_F_TEARDOWN(pop_back) {
    vector_free(utest_fixture->vector);
    tear_down_helpers();
}

UTEST_F(pop_back, removes_last_element) {
    utest_fixture->vector = vector_alloc(3, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);
    utest_fixture->vector->size = 3;

    ASSERT_EQ(VECTOR_SUCCESS, vector_pop_back(utest_fixture->vector));
    ASSERT_EQ((size_t)2, utest_fixture->vector->size);
}

UTEST_F(pop_back, does_nothing_when_vector_is_empty) {
    utest_fixture->vector = vector_alloc(3, sizeof(int), NULL);
    ASSERT_NE(NULL, utest_fixture->vector);

    ASSERT_EQ(VECTOR_SUCCESS, vector_pop_back(utest_fixture->vector));
    ASSERT_EQ((size_t)0, utest_fixture->vector->size);
}

UTEST(pop_back, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_pop_back(NULL));
}

UTEST_F(pop_back, calls_destructor_for_last_element) {
    const int expected[3] = {11, 22, 33};

    utest_fixture->vector = vector_alloc(3, sizeof(mock_t), &mock_destroy);
    ASSERT_NE(NULL, utest_fixture->vector);

    alloc_helpers(3);

    for (size_t i = 0; i < 3; i++)
        mock_emplace_back(utest_fixture->vector, expected[i]);

    ASSERT_EQ(VECTOR_SUCCESS, vector_pop_back(utest_fixture->vector));

    ASSERT_EQ((unsigned int)1, calls);
    ASSERT_EQ(expected[2], values[0]);
    ASSERT_EQ((uintptr_t)MOCK_OFFSET(utest_fixture->vector->data, 2),
              addresses[0]);
}