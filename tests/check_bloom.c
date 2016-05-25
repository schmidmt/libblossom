#include <stdlib.h>
#include <check.h>
#include <string.h>
#include "../src/blossom.h"

START_TEST(test_add)
{
    char * test_key = "test";
    size_t test_key_len = strlen(test_key);
    bloom_t * bloom = bloom_create(10, 0.01);
    ck_assert_ptr_ne(bloom, NULL);
    ck_assert_int_eq(bloom_add(bloom, test_key, test_key_len), 0);
    ck_assert_int_eq(bloom_get(bloom, test_key, test_key_len), 1);
    bloom_destroy(bloom);
}
END_TEST

START_TEST(test_get_not_added)
{
    char * test_key = "test";
    size_t test_key_len = strlen(test_key);
    bloom_t * bloom = bloom_create(10, 0.01);
    ck_assert_ptr_ne(bloom, NULL);
    ck_assert_int_eq(bloom_get(bloom, test_key, test_key_len), 0);
    bloom_destroy(bloom);
}
END_TEST

Suite * bloom_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("bloom");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_add);
    tcase_add_test(tc_core, test_get_not_added);
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
