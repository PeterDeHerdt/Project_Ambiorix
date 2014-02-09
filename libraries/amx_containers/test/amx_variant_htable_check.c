#define _BSD_SOURCE
#include <string.h>
#include <stdio.h>

#include <amx_containers/amx_variant.h>
#include "amx_containers_check.h"
#include "mock_malloc.h"
#include "mock_snprintf.h"
#include "mock_strdup.h"
#include "mock_amx_rbuffer_write.h"

static amx_var_t var_htable;
static amx_var_t var_htable2;
static amx_var_t var_htable3;
static amx_var_t var_null_htable;

	//                                      1         2         3         4         5         6         7         8         9
	//                             1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
	//                             key3=12345678901234567890123456789012345678901234567890123456789,key8==Hello
	//                             key3=12345678901234567890123456789012345678901234567890123456789,
static void amx_var_htable_checks_setup(void)
{
	amx_var_t *var = NULL;
	amx_var_init(&var_htable);
	var_htable.type_id = AMX_VAR_TYPE_ID_HTABLE;
	amx_htable_new(&var_htable.data.vm, 10);

	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_htable_insert(var_htable.data.vm, "key1", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_htable_insert(var_htable.data.vm, "Key2", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_htable_insert(var_htable.data.vm, "key3", &var->hit);

	amx_var_new(&var);
	amx_var_set_int32(var, 123);
	amx_htable_insert(var_htable.data.vm, "key4", &var->hit);

	amx_var_new(&var);
	amx_var_set_int32(var, -123);
	amx_htable_insert(var_htable.data.vm, "key5", &var->hit);

	amx_var_new(&var);
	amx_var_set_bool(var, true);
	amx_htable_insert(var_htable.data.vm, "key6", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890");
	amx_htable_insert(var_htable.data.vm, "key7", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890");
	amx_htable_insert(var_htable.data.vm, "key8", &var->hit);

	amx_var_init(&var_null_htable);
	var_null_htable.type_id = AMX_VAR_TYPE_ID_HTABLE;

	amx_var_init(&var_htable2);
	var_htable2.type_id = AMX_VAR_TYPE_ID_HTABLE;
	amx_htable_new(&var_htable2.data.vm, 10);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "1234567890123456789012345678901234567890123456789012345678901234");
	amx_htable_insert(var_htable2.data.vm, "key9", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "1234567890123456789012345678901234567890123456789012345678901234");
	amx_htable_insert(var_htable2.data.vm, "key10", &var->hit);

	amx_var_init(&var_htable3);
	var_htable3.type_id = AMX_VAR_TYPE_ID_HTABLE;
	amx_htable_new(&var_htable3.data.vm, 10);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "123456789012345678901234567890123456789012345678901234567890123");
	amx_htable_insert(var_htable3.data.vm, "key11", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, NULL);
	amx_htable_insert(var_htable3.data.vm, "key12", &var->hit);
}

static void amx_var_htable_checks_teardown(void)
{
	amx_var_clean(&var_htable);
	amx_var_clean(&var_htable2);
	amx_var_clean(&var_htable3);
	amx_var_clean(&var_null_htable);
}

START_TEST (amx_var_htable_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_string_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, "key3=World,key4=123,key5=-123,key6=true,key7=123456789012345678901234567890,key8=123456789012345678901234567890,Key2=Hello,key1=null");

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, "");

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_string_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	int retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	Expectation_strdup *exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(strdup);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_htable3, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	// fail writing key
	amx_var_t *var = amx_var_from_htable_it(amx_htable_get(var_htable.data.vm,"key3"));
	amx_var_set_string_copy(var, "1234567890123456789012345678901234567890123456789012345678");
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	// fail writing sep
	amx_var_set_string_copy(var, "12345678901234567890123456789012345678901234567890123456789");
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	// fail writing =
	amx_var_set_string_copy(var, "123456789012345678901234567890123456789012345678901234");
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	// fail writing null
	amx_var_set_string_copy(var, "12345678901234567890123456789012345678901234567890123");
	var = amx_var_from_htable_it(amx_htable_get(var_htable.data.vm,"key4"));
	amx_var_clean(var);
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = false;
	exp_malloc = ck_mock_add_expectation(malloc);
	exp_malloc->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_double_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_float_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_bool_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, false);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_invalid_type_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_to_llist_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 0);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_htable_to_llist_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_htable_to_htable_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 8);

	ck_assert_int_eq(amx_var_convert(&dest, &var_null_htable, AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 0);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_htable_to_htable_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_HTABLE);
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
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	//ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	Expectation_strdup *exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = false;
	exp_strdup = ck_mock_add_expectation(strdup);
	exp_strdup->fail = true;
	retval = amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(strdup);
	//ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_htable_to_fd)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_htable, AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_htable_copy_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_copy(&dest, &var_htable), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 8);

	ck_assert_int_eq(amx_var_copy(&dest, &var_null_htable), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 0);

	amx_var_clean(&dest);
}
END_TEST


START_TEST (amx_var_set_htable_null_check)
{
	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_htable_copy(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_htable_move(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_htable_copy(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_ptr_ne(variant.data.vm, NULL);
	ck_assert_int_eq(amx_htable_size(variant.data.vm), 0);
	ck_assert_int_eq(amx_var_set_htable_move(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_ptr_eq(variant.data.vm, NULL);
}
END_TEST


START_TEST (amx_var_set_htable_check)
{
	amx_htable_t *var_htable = NULL;
	ck_assert_int_eq(amx_htable_new(&var_htable, 10), 0);

	amx_var_t variant;
	amx_var_init(&variant);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_htable_insert(var_htable, "k1", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_htable_insert(var_htable, "k2", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_htable_insert(var_htable, "k3", &var->hit);

	ck_assert_int_eq(amx_var_set_htable_copy(&variant, var_htable), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_ptr_ne(variant.data.vm, NULL);
	ck_assert_ptr_ne(variant.data.vm, var_htable);
	ck_assert_int_eq(amx_htable_size(variant.data.vm), 3);

	ck_assert_int_eq(amx_var_set_htable_move(&variant, var_htable), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_ptr_eq(variant.data.vm, var_htable);
	ck_assert_int_eq(amx_htable_size(variant.data.vm), 3);

	amx_var_clean(&variant);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_set_htable_no_memory_check)
{
	amx_htable_t *var_htable = NULL;
	ck_assert_int_eq(amx_htable_new(&var_htable, 10), 0);

	amx_var_t variant;
	amx_var_init(&variant);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_htable_insert(var_htable, "K1", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_htable_insert(var_htable, "K2", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_htable_insert(var_htable, "K3", &var->hit);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_set_htable_copy(&variant, var_htable);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_set_htable_copy(&variant, var_htable);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_set_htable_copy(&variant, var_htable);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);

	amx_var_clean(&variant);
}
END_TEST
#endif

START_TEST (amx_var_get_htable_null_check)
{
	ck_assert_ptr_eq(amx_var_get_htable(NULL), NULL);
	ck_assert_ptr_eq((amx_htable_t *)amx_var_get_llist_da(NULL), NULL);
}
END_TEST

START_TEST (amx_var_get_htable_check)
{
	amx_var_t variant;
	amx_var_init(&variant);
	variant.type_id = AMX_VAR_TYPE_ID_UINT32;
	variant.data.ui32 = 2048;

	amx_htable_t *var_htable = amx_var_get_htable(&variant);
	ck_assert_ptr_ne(var_htable, NULL);
	ck_assert_int_eq(amx_htable_size(var_htable), 1);
	amx_htable_delete(&var_htable, amx_htable_var_delete);
	ck_assert_ptr_eq((amx_htable_t *)amx_var_get_htable_da(&variant), NULL);

	ck_assert_int_eq(amx_htable_new(&var_htable, 10), 0);

	amx_var_t *var = NULL;
	amx_var_new(&var);
	amx_var_set_string_move(var, NULL);
	amx_htable_insert(var_htable, "k1", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "Hello");
	amx_htable_insert(var_htable, "k2", &var->hit);

	amx_var_new(&var);
	amx_var_set_string_copy(var, "World");
	amx_htable_insert(var_htable, "k3", &var->hit);
	ck_assert_int_eq(amx_var_set_htable_move(&variant, var_htable), 0);

	ck_assert_ptr_eq((amx_htable_t *)amx_var_get_htable_da(&variant), var_htable);

	amx_var_clean(&variant);
}
END_TEST

Suite *amx_var_htable_suite(void)
{
	Suite *s = suite_create ("amx_variant_htable");
	TCase *tc = NULL;
	
	tc = tcase_create ("amx_var_htable_convert_checks");
	tcase_add_checked_fixture (tc, amx_var_htable_checks_setup, amx_var_htable_checks_teardown);
	tcase_add_test (tc, amx_var_htable_to_void_check);
	tcase_add_test (tc, amx_var_htable_to_string_check);
	tcase_add_test (tc, amx_var_htable_to_string_no_memory_check);
	tcase_add_test (tc, amx_var_htable_to_int8_check);
	tcase_add_test (tc, amx_var_htable_to_int16_check);
	tcase_add_test (tc, amx_var_htable_to_int32_check);
	tcase_add_test (tc, amx_var_htable_to_int64_check);
	tcase_add_test (tc, amx_var_htable_to_uint8_check);
	tcase_add_test (tc, amx_var_htable_to_uint16_check);
	tcase_add_test (tc, amx_var_htable_to_uint32_check);
	tcase_add_test (tc, amx_var_htable_to_uint64_check);
	tcase_add_test (tc, amx_var_htable_to_double_check);
	tcase_add_test (tc, amx_var_htable_to_float_check);
	tcase_add_test (tc, amx_var_htable_to_bool_check);
	tcase_add_test (tc, amx_var_htable_to_invalid_type_check);
	tcase_add_test (tc, amx_var_htable_to_llist_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_htable_to_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_htable_to_htable_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_htable_to_htable_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_htable_to_fd);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_htable_copy");
	tcase_add_checked_fixture (tc, amx_var_htable_checks_setup, amx_var_htable_checks_teardown);
	tcase_add_test (tc, amx_var_htable_copy_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_set_htable");
	tcase_add_test (tc, amx_var_set_htable_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_set_htable_no_memory_check);
#endif

	tcase_add_test (tc, amx_var_set_htable_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_htable");
	tcase_add_test (tc, amx_var_get_htable_null_check);
	tcase_add_test (tc, amx_var_get_htable_check);
	suite_add_tcase (s, tc);

	return s;
}
