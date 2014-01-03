#define _BSD_SOURCE
#include <string.h>

#include <amx_containers/amx_variant.h>
#include "amx_containers_check.h"
#include "mock_malloc.h"
#include "mock_snprintf.h"

amx_var_t var_double[20];

static void amx_var_double_checks_setup(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_init(&var_double[index]);
		var_double[index].type_id = AMX_VAR_TYPE_ID_DOUBLE;
	}
	var_double[0].data.d = 100;
	var_double[1].data.d = 0;
	var_double[2].data.d = 255;
	var_double[3].data.d = 1024;
	var_double[4].data.d = 2000.125;
	var_double[5].data.d = 1000.10;
}

static void amx_var_double_checks_teardown(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_clean(&var_double[index]);
	}
}

START_TEST (amx_var_double_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_string_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "100.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "0.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "255.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "1024.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "2000.125000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "1000.100000");

	ck_mock_reset(snprintf);
	Expectation_snprintf *exp = ck_mock_add_expectation(snprintf);
	exp->fail = true;
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_STRING), -1);
	ck_mock_reset(snprintf);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_double_to_string_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_double_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 1000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_double_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_float_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_bool_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, false);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_invalid_type_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[3], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[4], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_double[5], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_to_llist_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_double_to_llist_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_double_to_htable_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_double_to_htable_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_double_to_fd)
{
	amx_var_t dest;
	amx_var_init(&dest);

	//ck_assert_int_eq(amx_var_convert(&dest, &var_double[0], AMX_VAR_TYPE_ID_FD), -1);
	//ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	//ck_assert_int_eq(dest.data.fd, -1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_double[1], AMX_VAR_TYPE_ID_FD), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, 0);

	//ck_assert_int_eq(amx_var_convert(&dest, &var_double[2], AMX_VAR_TYPE_ID_FD), -1);
	//ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	//ck_assert_int_eq(dest.data.fd, -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_double_copy_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[0]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[0].data.d);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[1]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[1].data.d);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[2]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[2].data.d);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[3]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[3].data.d);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[4]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[4].data.d);

	ck_assert_int_eq(amx_var_copy(&dest, &var_double[5]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	ck_assert_int_eq(dest.data.d, var_double[5].data.d);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_set_double_null_check)
{
	ck_assert_int_eq(amx_var_set_double(NULL, 100), -1);
}
END_TEST

START_TEST (amx_var_set_double_check)
{
	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_double(&variant, 100), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_DOUBLE);
	//ck_assert_int_eq(variant.data.ui64, 100);

	amx_var_clean(&variant);
}
END_TEST

START_TEST (amx_var_get_double_null_check)
{
	ck_assert_int_eq(amx_var_get_double(NULL), 0);
}
END_TEST

START_TEST (amx_var_get_double_check)
{
	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_set_string_copy(&variant, "This is a line of text");
	//ck_assert_int_eq(amx_var_get_double(&variant), 0);

	amx_var_set_string_copy(&variant, "66");
	ck_assert_int_eq(amx_var_get_double(&variant), 66);

	amx_var_clean(&variant);
}
END_TEST

Suite *amx_var_double_suite(void)
{
	Suite *s = suite_create ("amx_variant_double");
	TCase *tc = NULL;
	
	tc = tcase_create ("amx_var_double_convert_checks");
	tcase_add_checked_fixture (tc, amx_var_double_checks_setup, amx_var_double_checks_teardown);
	tcase_add_test (tc, amx_var_double_to_void_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_double_to_string_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_double_to_string_check);
	tcase_add_test (tc, amx_var_double_to_int8_check);
	tcase_add_test (tc, amx_var_double_to_int16_check);
	tcase_add_test (tc, amx_var_double_to_int32_check);
	tcase_add_test (tc, amx_var_double_to_int64_check);
	tcase_add_test (tc, amx_var_double_to_uint8_check);
	tcase_add_test (tc, amx_var_double_to_uint16_check);
	tcase_add_test (tc, amx_var_double_to_uint32_check);
	tcase_add_test (tc, amx_var_double_to_uint64_check);
	tcase_add_test (tc, amx_var_double_to_float_check);
	tcase_add_test (tc, amx_var_double_to_double_check);
	tcase_add_test (tc, amx_var_double_to_bool_check);
	tcase_add_test (tc, amx_var_double_to_invalid_type_check);
	tcase_add_test (tc, amx_var_double_to_llist_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_double_to_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_double_to_htable_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_double_to_htable_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_double_to_fd);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_double_copy");
	tcase_add_checked_fixture (tc, amx_var_double_checks_setup, amx_var_double_checks_teardown);
	tcase_add_test (tc, amx_var_double_copy_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_set_double");
	tcase_add_test (tc, amx_var_set_double_null_check);
	tcase_add_test (tc, amx_var_set_double_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_double");
	tcase_add_test (tc, amx_var_get_double_null_check);
	tcase_add_test (tc, amx_var_get_double_check);
	suite_add_tcase (s, tc);

	return s;
}

