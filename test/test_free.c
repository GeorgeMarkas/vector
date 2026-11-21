#include "helpers.h"
#include "utest.h"
#include "vector.h"

struct free {
    Vector *vector;
};

UTEST_F_SETUP(free) {
    utest_fixture->vector = NULL;
    set_up_helpers();
}

UTEST_F_TEARDOWN(free) {
    vector_free(utest_fixture->vector);
    tear_down_helpers();
}

UTEST_F(free, calls_destructor_for_every_element) {
    const int expected[13] = {941, 36, 171, 79,  874, 320, 713,
                              233, 70, 157, 355, 466, 493};

    ASSERT_EQ(VECTOR_SUCCESS, vector_alloc(&utest_fixture->vector, 13,
                                           sizeof(mock_t), &mock_destroy));
    alloc_helpers(13);

    for (size_t i = 0; i < 13; i++)
        mock_emplace_back(utest_fixture->vector, expected[i]);

    const uintptr_t base = (uintptr_t)utest_fixture->vector->data;

    vector_free(utest_fixture->vector);
    utest_fixture->vector = NULL;

    ASSERT_EQ((unsigned int)13, calls);
    ASSERT_MEMEQ(expected, values, sizeof(expected));
    for (size_t i = 0; i < 13; i++)
        ASSERT_EQ(MOCK_OFFSET(base, i), addresses[i]);
}