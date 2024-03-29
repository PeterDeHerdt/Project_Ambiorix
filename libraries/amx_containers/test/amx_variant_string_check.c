#define _BSD_SOURCE
#include <string.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_variant.h>

#include "mock_malloc.h"
#include "mock_strdup.h"

amx_var_t var_string[20];

static void amx_var_string_checks_setup(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_init(&var_string[index]);
		var_string[index].type_id = AMX_VAR_TYPE_ID_STRING;
	}
	var_string[0].data.s = strdup("-100");
	var_string[1].data.s = strdup("0");
	var_string[2].data.s = strdup("+100");
	var_string[3].data.s = strdup("0xFF");
	var_string[4].data.s = strdup("1024");
	var_string[5].data.s = strdup("");
	var_string[6].data.s = NULL;
	var_string[7].data.s = strdup("true");
	var_string[8].data.s = strdup("YES");
	var_string[9].data.s = strdup("   0xFFFF");
	var_string[10].data.s = strdup("0xFFFFFFFF");
	var_string[11].data.s = strdup("0xFFFFFFFFFFFFFFFF");
	var_string[12].data.s = strdup("125A");
	var_string[13].data.s = strdup("-200");
	var_string[14].data.s = strdup("-33000");
	var_string[15].data.s = strdup("-2147483700");
	var_string[16].data.s = strdup("alpha,beta,delta,gamma,x-ray");
	var_string[17].data.s = strdup("key1=alpha,key2=beta,key3=delta,key4=gamma,key5=x-ray");
}

static void amx_var_string_checks_teardown(void)
{
	for(int index = 0; index < 20; index++)
	{
		amx_var_clean(&var_string[index]);
	}
}

START_TEST (amx_var_string_to_void_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[15], AMX_VAR_TYPE_ID_VOID), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_VOID);
	ck_assert_ptr_eq(dest.data.data, NULL);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_string_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[0].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[1].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[2].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[3].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[4].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[5].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[7].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[8].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[9].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[10].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[11].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[12].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[13].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[14].data.s);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[15], AMX_VAR_TYPE_ID_STRING), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[15].data.s);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_int8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, -100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_INT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[15], AMX_VAR_TYPE_ID_INT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT8);
	ck_assert_int_eq(dest.data.i8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_int16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, -100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0xFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_INT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, -200);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[15], AMX_VAR_TYPE_ID_INT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT16);
	ck_assert_int_eq(dest.data.i16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_int32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, -100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0xFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0xFFFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, -200);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_INT32), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, -33000);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[15], AMX_VAR_TYPE_ID_INT32), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT32);
	ck_assert_int_eq(dest.data.i32, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_int64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, -100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0xFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0xFFFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0xFFFFFFFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_INT64), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i64, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[13], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i32, -200);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[14], AMX_VAR_TYPE_ID_INT64), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_INT64);
	ck_assert_int_eq(dest.data.i32, -33000);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_uint8_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0xFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_UINT8), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_UINT8), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT8);
	ck_assert_int_eq(dest.data.ui8, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_uint16_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 100);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0xFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 1024);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_UINT16), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0xFFFF);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_UINT16), -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_UINT16);
	ck_assert_int_eq(dest.data.ui16, 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_uint32_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_UINT32), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_UINT32), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_UINT32), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_UINT32), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_UINT32), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_uint64_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_UINT64), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_UINT64), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_UINT64), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_UINT64), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_double_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_DOUBLE), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_DOUBLE), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_DOUBLE), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_DOUBLE), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_float_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_FLOAT), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_FLOAT), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_FLOAT), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_FLOAT), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_bool_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_BOOL), 0);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_BOOL), 0);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_llist_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[16], AMX_VAR_TYPE_ID_LIST), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_LIST);
	ck_assert_int_eq(amx_llist_size(dest.data.vl), 5);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_string_to_llist_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_LIST);
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
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

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
	retval = amx_var_convert(&dest, &var_string[16], AMX_VAR_TYPE_ID_LIST);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_string_to_htable_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 1);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[16], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 5);

	ck_assert_int_eq(amx_var_convert(&dest, &var_string[17], AMX_VAR_TYPE_ID_HTABLE), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_HTABLE);
	ck_assert_int_eq(amx_htable_size(dest.data.vm), 5);

	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_string_to_htable_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	ck_mock_reset(malloc);
	ck_mock_reset(strdup);

	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_HTABLE);
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
	retval = amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

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
	retval = amx_var_convert(&dest, &var_string[17], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	Expectation_strdup *strdup_exp = ck_mock_add_expectation(strdup);
	strdup_exp->fail = false;
	strdup_exp = ck_mock_add_expectation(strdup);
	strdup_exp->fail = false;
	strdup_exp = ck_mock_add_expectation(strdup);
	strdup_exp->fail = false;
	strdup_exp = ck_mock_add_expectation(strdup);
	strdup_exp->fail = true;
	retval = amx_var_convert(&dest, &var_string[17], AMX_VAR_TYPE_ID_HTABLE);
	ck_mock_reset(malloc);
	ck_assert_int_eq(retval, -1);

	amx_var_clean(&dest);
}
END_TEST
#endif

START_TEST (amx_var_string_to_fd_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_FD), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_FD), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_to_invalid_type_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[0], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[1], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[2], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[3], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[4], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[5], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[6], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[7], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[8], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[9], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[10], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[11], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_convert(&dest, &var_string[12], AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_string_copy_check)
{
	amx_var_t dest;
	amx_var_init(&dest);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[0]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[0].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[1]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[1].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[2]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[2].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[3]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[3].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[4]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[4].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[5]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[5].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[6]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[7]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[7].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[8]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[8].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[9]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[9].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[10]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[10].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[11]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[11].data.s);
	
	ck_assert_int_eq(amx_var_copy(&dest, &var_string[12]), 0);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(dest.data.s, var_string[12].data.s);
	
	amx_var_clean(&dest);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_string_copy_no_memory_check)
{
	amx_var_t dest;
	amx_var_init(&dest);

	// first malloc fails
	Expectation_strdup *exp = ck_mock_add_expectation(strdup);
	exp->fail = true;

	int retval = amx_var_copy(&dest, &var_string[11]);

	ck_mock_reset(strdup);
	ck_assert_int_eq (retval, -1);
	ck_assert_int_eq(dest.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(dest.data.s, NULL);
}
END_TEST
#endif

START_TEST (amx_var_set_string_null_check)
{
	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_string_copy(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_string_move(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_set_string_copy(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(variant.data.s, NULL);
	ck_assert_int_eq(amx_var_set_string_move(&variant, NULL), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_ptr_eq(variant.data.s, NULL);
}
END_TEST

START_TEST (amx_var_set_string_check)
{
	char *text = calloc(1,20);
	strcpy(text,"hallo world");

	amx_var_t variant;
	amx_var_init(&variant);

	ck_assert_int_eq(amx_var_set_string_copy(&variant, text), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(variant.data.s, "hallo world");

	ck_assert_int_eq(amx_var_set_string_move(&variant, text), 0);
	ck_assert_int_eq(variant.type_id, AMX_VAR_TYPE_ID_STRING);
	ck_assert_str_eq(variant.data.s, "hallo world");

	amx_var_clean(&variant);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_set_string_no_memory_check)
{
	char *text = calloc(1,20);
	strcpy(text,"hallo world");

	amx_var_t variant;
	amx_var_init(&variant);

	// first malloc fails
	Expectation_strdup *exp = ck_mock_add_expectation(strdup);
	exp->fail = true;
	int retval = amx_var_set_string_copy(&variant, text);

	ck_mock_reset(strdup);
	ck_assert_int_eq (retval, -1);

	amx_var_clean(&variant);
}
END_TEST
#endif

START_TEST (amx_var_get_string_null_check)
{
	ck_assert_ptr_eq(amx_var_get_string(NULL), NULL);
	ck_assert_ptr_eq((char *)amx_var_get_string_da(NULL), NULL);
}
END_TEST

START_TEST (amx_var_get_string_check)
{
	amx_var_t variant;
	amx_var_init(&variant);
	variant.type_id = AMX_VAR_TYPE_ID_UINT32;
	variant.data.ui32 = 2048;

	char *text = amx_var_get_string(&variant);
	ck_assert_str_eq(text, "2048");
	free(text);
	ck_assert_ptr_eq((char *)amx_var_get_string_da(&variant), NULL);

	ck_assert_int_eq(amx_var_set_string_copy(&variant,"Hello world"), 0);
	ck_assert_str_eq((char *)amx_var_get_string_da(&variant), "Hello world");
	text = amx_var_get_string(&variant);
	ck_assert_str_eq(text, "Hello world");
	free(text);

	amx_var_clean(&variant);
}
END_TEST

Suite *amx_var_string_suite(void)
{
	Suite *s = suite_create ("amx_variant_string");
	TCase *tc = NULL;
	
	tc = tcase_create ("amx_var_string_convert");
	tcase_add_checked_fixture (tc, amx_var_string_checks_setup, amx_var_string_checks_teardown);
	tcase_add_test (tc, amx_var_string_to_void_check);
	tcase_add_test (tc, amx_var_string_to_string_check);
	tcase_add_test (tc, amx_var_string_to_int8_check);
	tcase_add_test (tc, amx_var_string_to_int16_check);
	tcase_add_test (tc, amx_var_string_to_int32_check);
	tcase_add_test (tc, amx_var_string_to_int64_check);
	tcase_add_test (tc, amx_var_string_to_uint8_check);
	tcase_add_test (tc, amx_var_string_to_uint16_check);
	tcase_add_test (tc, amx_var_string_to_uint32_check);
	tcase_add_test (tc, amx_var_string_to_uint64_check);
	tcase_add_test (tc, amx_var_string_to_double_check);
	tcase_add_test (tc, amx_var_string_to_float_check);
	tcase_add_test (tc, amx_var_string_to_bool_check);
	tcase_add_test (tc, amx_var_string_to_llist_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_string_to_llist_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_string_to_htable_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_string_to_htable_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_string_to_fd_check);
	tcase_add_test (tc, amx_var_string_to_invalid_type_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_string_copy");
	tcase_add_checked_fixture (tc, amx_var_string_checks_setup, amx_var_string_checks_teardown);
	tcase_add_test (tc, amx_var_string_copy_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_string_copy_no_memory_check);
#endif
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_set_string");
	tcase_add_test (tc, amx_var_set_string_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_set_string_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_set_string_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_string");
	tcase_add_test (tc, amx_var_get_string_null_check);
	tcase_add_test (tc, amx_var_get_string_check);
	suite_add_tcase (s, tc);

	return s;
}
