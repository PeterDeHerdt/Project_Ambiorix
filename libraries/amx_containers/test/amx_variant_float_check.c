#define _BSD_SOURCE
#include <string.h>
#include <stdio.h>

#include <amx_containers/amx_variant.h>
#include "amx_containers_check.h"
#include "mock_malloc.h"
#include "mock_snprintf.h"

amx_var_t var_float[20];

static void amx_var_float_checks_setup(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_init(&var_float[index]);
		var_float[index].type_id = AMX_VAR_TYPE_ID_FLOAT;
	}
	var_float[0].data.f = 100;
	var_float[1].data.f = 0;
	var_float[2].data.f = 255;
	var_float[3].data.f = 1024;
	var_float[4].data.f = 2000.125;
	var_float[5].data.f = 1000.10;
	var_float[6].data.f = INT16_MIN;
	var_float[7].data.f = INT16_MAX;
	var_float[8].data.f = INT32_MIN;
	var_float[9].data.f = 0x7F000000;
	var_float[10].data.f = (float)-1e+32;
	var_float[11].data.f = (float)1e+32;
}

static void amx_var_float_checks_teardown(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_clean(&var_float[index]);
	}
}

START_TEST (amx_var_float_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_string_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "100.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "0.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "255.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "1024.000000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "2000.125000");

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	//ck_assert_str_eq(dest.data.s, "1000.100000");

	ck_mock_reset(snprintf);
	Expectation_snprintf *exp = ck_mock_add_expectation(snprintf);
	exp->fail = true;
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_STRING), -1);
	ck_mock_reset(snprintf);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_float_to_string_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_STRING);
	ck_mock_reset(malloc);

	ck_assert_int_eq(retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_float_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[6], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[7], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[8], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[9], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[10], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[6], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, INT16_MIN);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[7], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, INT16_MAX);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[8], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[9], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[10], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[6], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, INT16_MIN);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[7], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, INT16_MAX);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[8], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, INT32_MIN);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[9], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0x7F000000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[10], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[6], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, INT16_MIN);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[7], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, INT16_MAX);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[8], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, INT32_MIN);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[9], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0x7F000000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[10], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_UINT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT32);
	ck_assert_int_eq(dest.data.ui32, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 255);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 2000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 1000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_UINT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT64);
	ck_assert_int_eq(dest.data.ui64, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_double_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_DOUBLE);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_float_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_bool_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, false);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_BOOL);
	ck_assert_int_eq(dest.data.b, true);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_invalid_type_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[3], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[4], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_float[5], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_to_llist_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_float_to_llist_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_float_to_htable_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_float_to_htable_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_float_to_fd)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[0], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, -1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[1], AMX_VAR_TYPE_ID_FD), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[2], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, -1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_float[11], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FD);
	ck_assert_int_eq(dest.data.fd, -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_float_copy_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[0]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[0].data.f);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[1]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[1].data.f);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[2]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[2].data.f);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[3]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[3].data.f);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[4]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[4].data.f);

	ck_assert_int_eq(amx_var_copy(&dest, &var_float[5]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_FLOAT);
	ck_assert_int_eq(dest.data.f, var_float[5].data.f);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_set_float_null_check)
{
	ck_assert_int_eq(amx_var_set_float(NULL, 100), -1);
}
END_TEST

START_TEST (amx_var_set_float_check)
{
	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_float(&variant, 100), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_FLOAT);
	//ck_assert_int_eq(variant.data.ui64, 100);

	amx_var_clean(&variant);
}
END_TEST

START_TEST (amx_var_get_float_null_check)
{
	ck_assert_int_eq(amx_var_get_float(NULL), 0);
}
END_TEST

START_TEST (amx_var_get_float_check)
{
	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_set_string_copy(&variant, "This is a line of text");
	//ck_assert_int_eq(amx_var_get_float(&variant), 0);

	amx_var_set_string_copy(&variant, "66");
	ck_assert_int_eq(amx_var_get_float(&variant), 66);

	amx_var_clean(&variant);
}
END_TEST

Suite *amx_var_float_suite(void)
{
	Suite *s = suite_create ("amx_variant_float");
	TCase *tc = NULL;
	
	tc = tcase_create ("amx_var_float_convert_checks");
	tcase_add_checked_fixture (tc, amx_var_float_checks_setup, amx_var_float_checks_teardown);
	tcase_add_test (tc, amx_var_float_to_void_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_float_to_string_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_float_to_string_check);
	tcase_add_test (tc, amx_var_float_to_int8_check);
	tcase_add_test (tc, amx_var_float_to_int16_check);
	tcase_add_test (tc, amx_var_float_to_int32_check);
	tcase_add_test (tc, amx_var_float_to_int64_check);
	tcase_add_test (tc, amx_var_float_to_uint8_check);
	tcase_add_test (tc, amx_var_float_to_uint16_check);
	tcase_add_test (tc, amx_var_float_to_uint32_check);
	tcase_add_test (tc, amx_var_float_to_uint64_check);
	tcase_add_test (tc, amx_var_float_to_double_check);
	tcase_add_test (tc, amx_var_float_to_float_check);
	tcase_add_test (tc, amx_var_float_to_bool_check);
	tcase_add_test (tc, amx_var_float_to_invalid_type_check);
	tcase_add_test (tc, amx_var_float_to_llist_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_float_to_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_float_to_htable_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_float_to_htable_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_float_to_fd);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_float_copy");
	tcase_add_checked_fixture (tc, amx_var_float_checks_setup, amx_var_float_checks_teardown);
	tcase_add_test (tc, amx_var_float_copy_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_set_float");
	tcase_add_test (tc, amx_var_set_float_null_check);
	tcase_add_test (tc, amx_var_set_float_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_float");
	tcase_add_test (tc, amx_var_get_float_null_check);
	tcase_add_test (tc, amx_var_get_float_check);
	suite_add_tcase (s, tc);

	return s;
}
