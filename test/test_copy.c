#include "utest.h"
#include "vector.h"

struct copy {
    Vector *src;
    Vector *dest;
};

UTEST_F_SETUP(copy) {
    utest_fixture->src = NULL;
    utest_fixture->dest = NULL;
}

UTEST_F_TEARDOWN(copy) {
    vector_free(utest_fixture->src);
    vector_free(utest_fixture->dest);
}

UTEST_F(copy, creates_copy_of_given_vector) {
    const int expected[13] = {757, 28,  276,  560, 524, 364, 601,
                              984, 663, 1018, 78,  461, 455};
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->src, 13, sizeof(int), NULL));
    memcpy(utest_fixture->src->data, expected, sizeof(expected));
    utest_fixture->src->size = 13;

    ASSERT_EQ(VECTOR_SUCCESS,
              vector_copy(utest_fixture->src, &utest_fixture->dest));
    ASSERT_EQ((size_t)13, utest_fixture->dest->size);
    ASSERT_EQ(utest_fixture->src->capacity, utest_fixture->dest->capacity);
    ASSERT_EQ(sizeof(int), utest_fixture->dest->element_size);
    ASSERT_MEMEQ(expected, utest_fixture->dest->data, sizeof(expected));
    ASSERT_TRUE(utest_fixture->src->element_destructor ==
                utest_fixture->dest->element_destructor);
}

UTEST_F(copy, returns_error_code_when_vector_pointer_is_null) {
    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_copy(NULL, &utest_fixture->dest));
}

UTEST_F(copy, returns_error_code_when_vector_pointer_address_is_null) {
    ASSERT_EQ(VECTOR_SUCCESS,
              vector_alloc(&utest_fixture->src, 13, sizeof(int), NULL));

    ASSERT_EQ(VECTOR_ILLEGAL_ARG, vector_copy(utest_fixture->src, NULL));
}