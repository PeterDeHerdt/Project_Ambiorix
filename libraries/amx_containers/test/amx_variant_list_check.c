#define _BSD_SOURCE
#include <string.h>
#include <stdio.h>

#include <amx_containers/amx_variant.h>
#include "amx_containers_check.h"
#include "mock_malloc.h"
#include "mock_snprintf.h"
#include "mock_strdup.h"
#include "mock_amx_rbuffer_write.h"

static amx_var_t var_list;
static amx_var_t var_list2;
static amx_var_t var_list3;
static amx_var_t var_null_list;

static void amx_var_list_checks_setup(void)
{
	amx_var_t *var = NULL;
	amx_var_init(&var_list);
	var_list.type_id = AMX_VAR_TYPE_ID_LIST;
	amx_llist_new(&var_list.data.vl);

	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_llist_append(var_list.data.vl, &var->lit);

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

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890");
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890");
	amx_llist_append(var_list.data.vl, &var->lit);

	amx_var_init(&var_null_list);
	var_null_list.type_id = AMX_VAR_TYPE_ID_LIST;

	amx_var_init(&var_list2);
	var_list2.type_id = AMX_VAR_TYPE_ID_LIST;
	amx_llist_new(&var_list2.data.vl);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "1234567890123456789012345678901234567890123456789012345678901234");
	amx_llist_append(var_list2.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "1234567890123456789012345678901234567890123456789012345678901234");
	amx_llist_append(var_list2.data.vl, &var->lit);

	amx_var_init(&var_list3);
	var_list3.type_id = AMX_VAR_TYPE_ID_LIST;
	amx_llist_new(&var_list3.data.vl);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890123456789012345678901234567890123");
	amx_llist_append(var_list3.data.vl, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, NULL);
	amx_llist_append(var_list3.data.vl, &var->lit);
}

static void amx_var_list_checks_teardown(void)
{
	amx_var_clean(&var_list);
	amx_var_clean(&var_list2);
	amx_var_clean(&var_list3);
	amx_var_clean(&var_null_list);
}

START_TEST (amx_var_list_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_list, AMX_VAR_TYPE_ID_VOID), 0);
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
	ck_assert_str_eq(dest.data.s, "null,Hello,World,123,-123,true,123456789012345678901234567890,123456789012345678901234567890");

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_list, AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, "");

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_list_to_string_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	int retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	Expectation_strdup *exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(strdup);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_list2, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_list3, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_list_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 8);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_list_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 8);

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
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_list, AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 0);

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
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_list, AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 0);

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
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_HTABLE);
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

	Expectation_strdup *exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = false;
	exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = true;
	retval = amx_var_convert(&dest, &var_list, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(strdup);
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
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 8);

	ck_assert_int_eq(amx_var_copy(&dest, &var_null_list), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_set_llist_null_check)
{
	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_llist_copy(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_llist_move(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_llist_copy(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_ptr_ne(variant.data.vl, NULL);
	ck_assert_int_eq(amx_llist_size(variant.data.vl), 0);
	ck_assert_int_eq(amx_var_set_llist_move(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_ptr_eq(variant.data.vl, NULL);
}
END_TEST

START_TEST (amx_var_set_llist_check)
{
	amx_llist_t *var_list = NULL;
	ck_assert_int_eq(amx_llist_new(&var_list), 0);

	amx_var_t variant;
	amx_var_init(&variant);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_llist_append(var_list, &var->lit);

	ck_assert_int_eq(amx_var_set_llist_copy(&variant, var_list), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_ptr_ne(variant.data.vl, NULL);
	ck_assert_ptr_ne(variant.data.vl, var_list);
	ck_assert_int_eq(amx_llist_size(variant.data.vl), 3);

	ck_assert_int_eq(amx_var_set_llist_move(&variant, var_list), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_ptr_eq(variant.data.vl, var_list);
	ck_assert_int_eq(amx_llist_size(variant.data.vl), 3);

	amx_var_clean(&variant);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_set_llist_no_memory_check)
{
	amx_llist_t *var_list = NULL;
	ck_assert_int_eq(amx_llist_new(&var_list), 0);

	amx_var_t variant;
	amx_var_init(&variant);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_llist_append(var_list, &var->lit);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_set_llist_copy(&variant, var_list);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_set_llist_copy(&variant, var_list);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_set_llist_copy(&variant, var_list);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	amx_var_clean(&variant);
}
END_TEST
#endif

START_TEST (amx_var_get_llist_null_check)
{
	ck_assert_ptr_eq(amx_var_get_llist(NULL), NULL);
	ck_assert_ptr_eq((amx_llist_t *)amx_var_get_llist_da(NULL), NULL);
}
END_TEST

START_TEST (amx_var_get_llist_check)
{
	amx_var_t variant;
	amx_var_init(&variant);
	variant.type_id = AMX_VAR_TYPE_ID_UINT32;
	variant.data.ui32 = 2048;

	amx_llist_t *var_list = amx_var_get_llist(&variant);
	ck_assert_ptr_ne(var_list, NULL);
	ck_assert_int_eq(amx_llist_size(var_list), 1);
	amx_llist_delete(&var_list, amx_llist_var_delete);
	ck_assert_ptr_eq((amx_llist_t *)amx_var_get_llist_da(&variant), NULL);

	ck_assert_int_eq(amx_llist_new(&var_list), 0);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_llist_append(var_list, &var->lit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_llist_append(var_list, &var->lit);
	ck_assert_int_eq(amx_var_set_llist_move(&variant, var_list), 0);

	ck_assert_ptr_eq((amx_llist_t *)amx_var_get_llist_da(&variant), var_list);

	amx_var_clean(&variant);
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
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_list_to_string_no_memory_check);
#endif
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

	tc = tcase_create ("amx_var_set_llist");
	tcase_add_test (tc, amx_var_set_llist_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_set_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_set_llist_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_llist");
	tcase_add_test (tc, amx_var_get_llist_null_check);
	tcase_add_test (tc, amx_var_get_llist_check);
	suite_add_tcase (s, tc);

	return s;
}
