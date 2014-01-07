#define _BSD_SOURCE
#define _POSIX_SOURCE
#include <stdlib.h>
#include <check.h>
#include <stdio.h>

#include "mock_malloc.h"
#include "mock_strdup.h"
#include "mock_amx_htable_insert.h"
#include <amx_parser.h>



int dummy_success_parse_function(int fd, amx_var_t *result)
{
    (void)fd;
    (void)result;
    return 0;
}

int dummy_failed_parse_function(int fd, amx_var_t *result)
{
    (void)fd;
    (void)result;
    return -1;
}

int dummy_success_verify_function(int fd, bool *result)
{
    (void)fd;
    (void)result;
    return 0;
}


int dummy_failed_verify_function(int fd, bool *result)
{
    (void)fd;
    (void)result;
    return -1;
}


START_TEST (parser_store_parser_new_and_delete)
{
    amx_parser_store_parser_t *parser = NULL;
    amx_parser_store_parser_t *parser1 = NULL;
    amx_parser_store_parser_delete(&parser);
    ck_assert_int_eq( amx_parser_store_parser_new(NULL, NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_new(NULL, "test"), -1);
    ck_assert_int_eq( amx_parser_store_parser_new(&parser, NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_new(&parser, "test"), 0);
    ck_assert_int_eq( amx_parser_store_parser_new(&parser1, "test1"), 0);
    amx_parser_store_parser_delete(NULL);
    amx_parser_store_parser_delete(&parser);
    amx_parser_store_parser_delete(&parser1);

    amx_parser_store_parser_t parser2;
    ck_assert_int_eq( amx_parser_store_parser_init(NULL, NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_init(NULL, "test2"), -1);
    ck_assert_int_eq( amx_parser_store_parser_init(&parser2, NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_init(&parser2, "test2"), 0);
    amx_parser_store_parser_clean(NULL);
    amx_parser_store_parser_clean(&parser2);
}
END_TEST

START_TEST (parser_store_parser_set_functions)
{
    amx_parser_store_parser_t parser;
    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "test"), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,NULL), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,dummy_success_parse_function), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,NULL), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,NULL), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,dummy_success_verify_function), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,NULL), 0);
    amx_parser_store_parser_clean(&parser);
}
END_TEST

START_TEST (parser_store_register_and_unregister)
{
    amx_parser_store_parser_t parser;
    amx_parser_store_parser_t parser1;
    amx_parser_store_parser_t parser2;
    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "test"), 0);
    ck_assert_int_eq( amx_parser_store_parser_init(&parser1, "test1"), 0);
    ck_assert_int_eq( amx_parser_store_parser_init(&parser2, "test2"), 0);
    ck_assert_int_eq( amx_parser_store_register_parser(NULL), -1);
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), 0);
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), -1);
    ck_assert_int_eq( amx_parser_store_register_parser(&parser1), 0);
    ck_assert_int_eq( amx_parser_store_unregister_parser(NULL), -1);
    ck_assert_int_eq( amx_parser_store_unregister_parser(&parser), 0);
    ck_assert_int_eq( amx_parser_store_unregister_parser(&parser2), -1);
    ck_assert_int_eq( amx_parser_store_unregister_parser(&parser1), 0);
    ck_assert_int_eq( amx_parser_store_unregister_parser(&parser2), -1);
    amx_parser_store_parser_clean(&parser);
    amx_parser_store_parser_clean(&parser1);
    amx_parser_store_parser_clean(&parser2);
}
END_TEST

START_TEST (parser_store_parse)
{
    amx_parser_store_parser_t parser;
    amx_var_t result;
    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "test"), 0);
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), 0);
    ck_assert_int_eq( amx_parser_store_parse_file(NULL,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_file("test.txt",NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_file(NULL,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_file(NULL,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_file("text.txt","test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_file(NULL,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_file("notexisting.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_file("text.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,dummy_success_parse_function), 0);
    ck_assert_int_eq( amx_parser_store_parse_file("text.txt","test",&result), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,dummy_failed_parse_function), 0);
    ck_assert_int_eq( amx_parser_store_parse_file("text.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_file("text.txt","notexisting",&result), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,dummy_success_parse_function), 0);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,"test",&result), 0);
    ck_assert_int_eq( amx_parser_store_parse_fd(0,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(0,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(0,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(0,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_parse_function(&parser,NULL), 0);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parse_fd(1,"notexisting",&result), -1);

    amx_parser_store_parser_clean(&parser);
}
END_TEST


START_TEST (parser_store_verify)
{
    amx_parser_store_parser_t parser;
    bool result;
    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "test"), 0);
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), 0);
    ck_assert_int_eq( amx_parser_store_verify_file(NULL,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_file("test.txt",NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_file(NULL,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_file(NULL,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_file("text.txt","test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_file(NULL,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_file("notexisting.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_file("text.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,dummy_success_verify_function), 0);
    ck_assert_int_eq( amx_parser_store_verify_file("text.txt","test",&result), 0);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,dummy_failed_verify_function), 0);
    ck_assert_int_eq( amx_parser_store_verify_file("text.txt","test",&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_file("text.txt","notexisting",&result), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,dummy_success_verify_function), 0);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,"test",&result), 0);
    ck_assert_int_eq( amx_parser_store_verify_fd(0,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,NULL,NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(0,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(0,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,NULL,&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(0,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,"test",NULL), -1);
    ck_assert_int_eq( amx_parser_store_parser_set_verify_function(&parser,NULL), 0);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,"test",&result), -1);
    ck_assert_int_eq( amx_parser_store_verify_fd(1,"notexisting",&result), -1);

    amx_parser_store_parser_clean(&parser);
}
END_TEST


#ifdef MOCK_MALLOC
START_TEST (parser_store_no_memory)
{
    amx_parser_store_parser_t parser;
    amx_parser_store_parser_t *parser1;

    Expectation_strdup *strdup_exp = ck_mock_add_expectation(strdup);
    strdup_exp->fail = true;
    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "malloc"), -1);
    ck_mock_reset(strdup);

    Expectation_malloc *malloc_exp = ck_mock_add_expectation(malloc);
    malloc_exp->fail = true;
    ck_assert_int_eq( amx_parser_store_parser_new(&parser1, "malloc1"), -1);


    ck_assert_int_eq( amx_parser_store_parser_init(&parser, "malloc"), 0);
    malloc_exp = ck_mock_add_expectation(malloc);
    malloc_exp->fail = true;
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), -1);


    Expectation_amx_htable_insert *amx_htable_insert_exp = ck_mock_add_expectation(amx_htable_insert);
    amx_htable_insert_exp->fail = true;
    ck_assert_int_eq( amx_parser_store_register_parser(&parser), -1);
    ck_mock_reset(amx_htable_insert);

    amx_parser_store_parser_clean(&parser);
    ck_mock_reset(malloc);

}
END_TEST
#endif

Suite *amx_parser_store_suite(void)
{
    Suite *s = suite_create ("amx_parser_store");

    /* Core test case */
    TCase *tc = tcase_create ("amx parser store parser tests");
    tcase_add_test (tc, parser_store_parser_new_and_delete);
    tcase_add_test (tc, parser_store_parser_set_functions);
    tcase_add_test (tc, parser_store_register_and_unregister);
    tcase_add_test (tc, parser_store_parse);
    tcase_add_test (tc, parser_store_verify);
#ifdef MOCK_MALLOC
    tcase_add_test (tc, parser_store_no_memory);
#endif
    suite_add_tcase (s, tc);

    return s;
}

int main (void)
{
    int number_failed;
    Suite *s = amx_parser_store_suite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
