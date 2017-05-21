#include <stdlib.h>
#include <check.h>
#include "../src/bitvector.h"

START_TEST(test_init)
{
    
    bitvector_t * vector = bv_create(sizeof(int) * CHAR_BIT);
    ck_assert_msg(vector != 0, "Initial vector should not be NULL");
    for(size_t i = 0; i < sizeof(int) * CHAR_BIT; ++i)
    {
        ck_assert_int_eq(bv_get(vector, i), 0);
    }
    bv_destroy(vector);
}
END_TEST


START_TEST(test_set)
{
    bitvector_t * vector = bv_create(sizeof(int) * CHAR_BIT);
    ck_assert_msg(vector != 0, "Initial vector should not be NULL");
    bv_set(vector, 0);
    ck_assert_int_eq(bv_get(vector, 0), 1);
    bv_destroy(vector);
}
END_TEST

START_TEST(test_unset)
{

    bitvector_t * vector = bv_create(sizeof(int) * CHAR_BIT);
    ck_assert_msg(vector != 0, "Initial vector should not be NULL");
    bv_set(vector, 0);
    bv_unset(vector, 0);
    ck_assert_int_eq(bv_get(vector, 0), 0);
    bv_destroy(vector);
}
END_TEST


Suite * bloom_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("bitvector");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_set);
    tcase_add_test(tc_core, test_unset);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = bloom_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_ENV);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
