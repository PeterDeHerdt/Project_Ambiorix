#define _BSD_SOURCE
#include <string.h>

#include <amx_containers/amx_variant.h>
#include "amx_containers_check.h"
#include "mock_malloc.h"
#include "mock_snprintf.h"

amx_var_t var_list;

static void amx_var_list_checks_setup(void)
{
	amx_var_t *var = NULL;
	amx_var_init(&var_list);
	var_list.type_id = AMX_VAR_TYPE_ID_LIST;
	amx_llist_new(&var_list.data.vl);
	
	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_int32(var, 123);
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_int32(var, -123);
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_bool(var, true);
	amx_llist_append(var_list.data.vl, &var->lit);
}

static void amx_var_list_checks_teardown(void)
{
	amx_var_clean(&var_list);
}

START_TEST (amx_var_list_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_string_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, "Hello,World,123,-123,true");

	amx_var_clean(&dest);
}
END_TEST


START_TEST (amx_var_list_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 5);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_double_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_float_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_bool_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_invalid_type_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_llist_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 5);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_list_to_llist_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_list_to_htable_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 5);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_list_to_htable_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_list_to_fd)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_copy_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_copy(&dest, &var_list), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 5);

	amx_var_clean(&dest);
}
END_TEST

Suite *amx_var_list_suite(void)
{
	Suite *s = suite_create ("amx_variant_list");
	TCase *tc = NULL;
	
	tc = tcase_create ("amx_var_list_convert_checks");
	tcase_add_checked_fixture (tc, amx_var_list_checks_setup, amx_var_list_checks_teardown);
	tcase_add_test (tc, amx_var_list_to_void_check);
	tcase_add_test (tc, amx_var_list_to_string_check);
	tcase_add_test (tc, amx_var_list_to_int8_check);
	tcase_add_test (tc, amx_var_list_to_int16_check);
	tcase_add_test (tc, amx_var_list_to_int32_check);
	tcase_add_test (tc, amx_var_list_to_int64_check);
	tcase_add_test (tc, amx_var_list_to_uint8_check);
	tcase_add_test (tc, amx_var_list_to_uint16_check);
	tcase_add_test (tc, amx_var_list_to_uint32_check);
	tcase_add_test (tc, amx_var_list_to_uint64_check);
	tcase_add_test (tc, amx_var_list_to_double_check);
	tcase_add_test (tc, amx_var_list_to_float_check);
	tcase_add_test (tc, amx_var_list_to_bool_check);
	tcase_add_test (tc, amx_var_list_to_invalid_type_check);
	tcase_add_test (tc, amx_var_list_to_llist_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_list_to_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_list_to_htable_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_list_to_htable_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_list_to_fd);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_list_copy");
	tcase_add_checked_fixture (tc, amx_var_list_checks_setup, amx_var_list_checks_teardown);
	tcase_add_test (tc, amx_var_list_copy_check);
	suite_add_tcase (s, tc);

	return s;
}
