#include "utest.h"
#include "vector.h"

struct at {
    Vector *vector;
};

UTEST_F_SETUP(at) {
    utest_fixture->vector = NULL;
}

UTEST_F_TEARDOWN(at) {
    vector_free(utest_fixture->vector);
}

UTEST_F(at, accesses_element_at_given_pos) {
    const int expected[13] = {534, 168, 1003, 1017, 226, 318, 908,
                              77,  607, 382,  873,  122, 76};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));
    memcpy(utest_fixture->vector->data, expected, sizeof(expected));
    utest_fixture->vector->size = 13;

    for (size_t i = 0; i < 13; i++)
        ASSERT_EQ(expected[i], *(int *)vector_at(utest_fixture->vector, i));
}

UTEST(at, returns_null_when_vector_pointer_is_null) {
    ASSERT_EQ(NULL, vector_at(NULL, 0));
}

UTEST_F(at, returns_null_when_pos_is_out_of_range) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->vector, 13, sizeof(int)));

    const size_t size = utest_fixture->vector->size;
    ASSERT_EQ(NULL, vector_at(utest_fixture->vector, size));
    ASSERT_EQ(NULL, vector_at(utest_fixture->vector, size + 1));
}