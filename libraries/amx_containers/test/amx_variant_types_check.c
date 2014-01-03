#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_variant.h>

#include "mock_malloc.h"

static unsigned int check_copy = 0;
//static unsigned int test_type_convert = 0;
//static unsigned int test_type_compare = 0;
static unsigned int check_delete = 0;


typedef struct _test_type1 {
	char *name;
	char *email;
	unsigned int age;
} test_type1_t;

static void test_type1_delete(amx_var_t *var);
static int test_type1_copy(amx_var_t *dest, const amx_var_t *src);

static amx_var_type_t amx_var_test_type1 = 
{
	.copy = test_type1_copy,
	.convert = NULL,
	.compare = NULL,
	.del = test_type1_delete,
	.name = "test_type"
};

static void test_type1_delete(amx_var_t *var)
{
	test_type1_t *data = amx_var_get_data_da(var);
	free(data->name);
	free(data->email);
	free(data);
	check_delete++;
}

static int test_type1_copy(amx_var_t *dest, const amx_var_t *src)
{
	int retval = -1;
	test_type1_t *src_data = amx_var_get_data_da(src);
	test_type1_t *dst_data = calloc(1, sizeof(test_type1_t));
	if (!dst_data)
	{
		goto exit;
	}

	amx_var_set_data_move(dest, dst_data, amx_var_test_type1.type_id);

	dst_data->age = src_data->age;
	printf("name = %s\n", src_data->name);
	dst_data->name = strdup(src_data->name);
	if (!dst_data->name)
	{
		amx_var_clean(dest);
		goto exit;
	}
	printf("email = %s\n", src_data->email);
	dst_data->email = strdup(src_data->email);
	if (!dst_data->name)
	{
		amx_var_clean(dest);
		goto exit;
	}
	check_copy++;

	retval = 0;
exit:
	return retval;
}

static void amx_var_test_type_delete(amx_var_t *var)
{
	(void)var;
	check_delete++;
}

static amx_var_type_t amx_var_test_type2 = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = amx_var_test_type_delete,
	.name = "test_type"
};

static amx_var_type_t amx_var_test_type3 = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = amx_var_test_type_delete,
	.name = "test_type_3"
};

static amx_var_type_t amx_var_empty_name_type2 = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = amx_var_test_type_delete,
	.name = ""
};

test_type1_t *test_data = NULL;
amx_var_t test_var;

static void amx_var_type_checks_setup(void)
{
	ck_assert_int_ne (amx_var_register_type(&amx_var_test_type1), -1);

	// create a data structure
	test_data = calloc(1, sizeof(test_type1_t));
	test_data->age = 44;
	test_data->name = strdup("Peter De Herdt");
	test_data->email = strdup("Peter.DeHerdt@telenet.be");

	amx_var_init(&test_var);
	amx_var_set_data_move(&test_var, test_data, amx_var_test_type1.type_id);
}

static void amx_var_type_checks_teardown(void)
{
	amx_var_clean(&test_var);
	test_data = NULL;

	ck_assert_int_eq (amx_var_unregister_type(&amx_var_test_type1), 0);
}

START_TEST (amx_var_register_type_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_var_register_type(NULL), -1);
	// empty name types must not be accepted
	ck_assert_int_eq (amx_var_register_type(&amx_var_empty_name_type2), -1);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_register_type_no_memory_check)
{
	// first malloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_register_type(&amx_var_test_type1);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
}
END_TEST
#endif

START_TEST (amx_var_register_type_check)
{
	// register a type
	int retval = amx_var_register_type(&amx_var_test_type1);
	ck_assert_int_ge (retval, AMX_VAR_TYPE_ID_CUSTOM_BASE);
	ck_assert_int_eq (amx_var_test_type1.type_id, retval);
	ck_assert_ptr_ne (amx_var_test_type1.hit.ait, NULL);
	ck_assert_str_eq (amx_var_test_type1.hit.key, amx_var_test_type1.name);

	// register the same type must return the same id
	ck_assert_int_eq (amx_var_register_type(&amx_var_test_type1), retval);

	retval = amx_var_register_type(&amx_var_test_type3);
	ck_assert_int_ge (retval, AMX_VAR_TYPE_ID_CUSTOM_BASE + 1);
	ck_assert_int_eq (amx_var_test_type3.type_id, retval);
	ck_assert_ptr_ne (amx_var_test_type3.hit.ait, NULL);
	ck_assert_str_eq (amx_var_test_type3.hit.key, amx_var_test_type3.name);

	// register a type with the same name must fail
	ck_assert_int_eq (amx_var_register_type(&amx_var_test_type2), -1);

	// remove the types here
	amx_var_unregister_type(&amx_var_test_type1);
	amx_var_unregister_type(&amx_var_test_type3);
}
END_TEST

START_TEST (amx_var_unregister_type_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_var_unregister_type(NULL), -1);
}
END_TEST

START_TEST (amx_var_unregister_type_check)
{
	// register a type
	int type_id = amx_var_register_type(&amx_var_test_type1);

	// unregister a type with id < AMX_VAR_TYPE_ID_CUSTOM_BASE must fail
	ck_assert_int_eq (amx_var_unregister_type(&amx_var_test_type2), -1);

	// unregister a type with the same name but different type must fail, even if the type_id is faked
	amx_var_test_type2.type_id = type_id;
	ck_assert_int_eq (amx_var_unregister_type(&amx_var_test_type2), -1);

	// unregistering a not registered type must fail
	amx_var_test_type3.type_id = type_id + 1;
	ck_assert_int_eq (amx_var_unregister_type(&amx_var_test_type3), -1);

	// remove the type here
	ck_assert_int_eq (amx_var_unregister_type(&amx_var_test_type1), 0);
}
END_TEST

START_TEST (amx_var_type_name_invalid_type_id_check)
{
	amx_var_register_type(&amx_var_test_type1);

	ck_assert_ptr_eq((char *)amx_var_get_type_name_from_id(AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), NULL);
	ck_assert_ptr_eq((char *)amx_var_get_type_name_from_id(AMX_VAR_TYPE_ID_CUSTOM_BASE + 1), NULL);

	amx_var_unregister_type(&amx_var_test_type1);
}
END_TEST

START_TEST (amx_var_type_name_valid_type_id_check)
{
	int type_id = amx_var_register_type(&amx_var_test_type1);

	ck_assert_str_eq(amx_var_get_type_name_from_id(type_id), "test_type");

	amx_var_unregister_type(&amx_var_test_type1);
}
END_TEST

START_TEST (amx_var_type_id_invalid_name_check)
{
	ck_assert_int_eq(amx_var_get_type_id_from_name(NULL), -1);
	ck_assert_int_eq(amx_var_get_type_id_from_name(""), -1);
	ck_assert_int_eq(amx_var_get_type_id_from_name("invalid_name_type"), -1);
}
END_TEST

START_TEST (amx_var_type_id_valid_name_id_check)
{
	int type_id = amx_var_register_type(&amx_var_test_type1);

	ck_assert_int_eq(amx_var_get_type_id_from_name("test_type"), type_id);
	ck_assert_int_eq(amx_var_get_type_id_from_name(AMX_VAR_TYPE_NAME_VOID), AMX_VAR_TYPE_ID_VOID);
	ck_assert_int_eq(amx_var_get_type_id_from_name(AMX_VAR_TYPE_NAME_STRING), AMX_VAR_TYPE_ID_STRING);

	amx_var_unregister_type(&amx_var_test_type1);
}
END_TEST

START_TEST (amx_var_set_data_null_check)
{
	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_set_data_move(NULL, NULL, amx_var_test_type1.type_id), -1);
	ck_assert_int_eq(amx_var_set_data_move(&myvar, NULL, amx_var_test_type1.type_id), 0);

	ck_assert_int_eq(amx_var_set_data_copy(NULL, NULL, amx_var_test_type1.type_id), -1);
	ck_assert_int_eq(amx_var_set_data_copy(&myvar, NULL, amx_var_test_type1.type_id), 0);

	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_set_data_move_check)
{
	test_type1_t *data = calloc(1, sizeof(test_type1_t));
	data->age = 25;
	data->name = strdup("John Smith");
	data->email = strdup("John.Smith@monster.com");

	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_set_data_move(&myvar, data, amx_var_test_type1.type_id), 0);
	ck_assert_ptr_eq(myvar.data.data, data);
	ck_assert_int_eq(myvar.type_id, amx_var_test_type1.type_id);

	ck_assert_ptr_eq(amx_var_get_data_da(&myvar), data);

	// this will free the memory allocated for the data
	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_set_data_copy_check)
{
	test_type1_t *data = calloc(1, sizeof(test_type1_t));
	data->age = 25;
	data->name = strdup("John Smith");
	data->email = strdup("John.Smith@monster.com");

	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_set_data_copy(&myvar, data, amx_var_test_type1.type_id), 0);
	ck_assert_ptr_ne(myvar.data.data, NULL);
	ck_assert_ptr_ne(myvar.data.data, data);
	ck_assert_int_eq(myvar.type_id, amx_var_test_type1.type_id);
	test_type1_t *copy = myvar.data.data;
	ck_assert_str_eq(copy->name, "John Smith");
	ck_assert_str_eq(copy->email, "John.Smith@monster.com");
	ck_assert_int_eq(copy->age, 25);

	ck_assert_ptr_ne(amx_var_get_data_da(&myvar), NULL);

	// this will free the memory allocated for the data in the variant
	amx_var_clean(&myvar);
	
	// free the data here
	free(data->name);
	free(data->email);
	free(data);
}
END_TEST

START_TEST (amx_var_set_data_fixed_types_check)
{
	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_set_data_move(&myvar, NULL, AMX_VAR_TYPE_ID_STRING), -1);
	ck_assert_int_eq(amx_var_set_data_copy(&myvar, NULL, AMX_VAR_TYPE_ID_STRING), -1);

	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_set_data_invalid_type_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_set_data_move(&myvar, NULL, AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);
	ck_assert_int_eq(amx_var_set_data_copy(&myvar, NULL, AMX_VAR_TYPE_ID_CUSTOM_BASE + 100), -1);

	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_get_data_null_check)
{
	ck_assert_ptr_eq(amx_var_get_data_da(NULL), NULL);
}
END_TEST

START_TEST (amx_var_get_data_fixed_types_check)
{
	amx_var_t myvar;
	amx_var_init(&myvar); // void type

	ck_assert_ptr_eq(amx_var_get_data_da(&myvar), NULL);

	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_type_copy_null_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);

	ck_assert_int_eq(amx_var_copy(NULL, NULL), -1);
	ck_assert_int_eq(amx_var_copy(&myvar, NULL), -1);
	ck_assert_int_eq(amx_var_copy(NULL, &myvar), -1);

	amx_var_clean(&myvar);
}
END_TEST

START_TEST (amx_var_type_copy_check)
{
	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &test_var), 0);
	ck_assert_ptr_ne(copyvar.data.data, NULL);
	ck_assert_int_eq(copyvar.type_id, test_var.type_id);
	test_type1_t *test_data = test_var.data.data;
	test_type1_t *copy_data = copyvar.data.data;
	ck_assert_str_eq(copy_data->email, test_data->email);
	ck_assert_str_eq(copy_data->name, test_data->name);
	ck_assert_int_eq(copy_data->age, test_data->age);

	// clean the variants
	amx_var_clean(&copyvar);
}
END_TEST

START_TEST (amx_var_type_copy_invalid_type_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);
	myvar.type_id = AMX_VAR_TYPE_ID_CUSTOM_BASE + 100;

	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &myvar), -1);
	ck_assert_ptr_eq(copyvar.data.data, NULL);

	// clean the variants
	amx_var_clean(&myvar);
	amx_var_clean(&copyvar);
}
END_TEST

START_TEST (amx_var_type_copy_no_copy_fn_check)
{
	ck_assert_int_ge(amx_var_register_type(&amx_var_test_type3), AMX_VAR_TYPE_ID_CUSTOM_BASE);
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);
	myvar.type_id = amx_var_test_type3.type_id;

	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &myvar), -1);
	ck_assert_ptr_eq(copyvar.data.data, NULL);

	// clean the variants
	amx_var_clean(&myvar);
	amx_var_clean(&copyvar);

	amx_var_unregister_type(&amx_var_test_type3);
}
END_TEST

START_TEST (amx_var_type_convert_null_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_t dest;
	amx_var_init(&myvar);
	amx_var_init(&dest);

    ck_assert_int_eq(amx_var_convert(&dest, &myvar, amx_var_test_type1.type_id), -1);
    ck_assert_int_eq(amx_var_convert(&dest, NULL, amx_var_test_type1.type_id), -1);

	amx_var_clean(&myvar);
	amx_var_clean(&dest);
}
END_TEST

START_TEST (amx_var_type_convert_custom_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);

	myvar.type_id = AMX_VAR_TYPE_ID_STRING;
	myvar.data.s = strdup("test");
	ck_assert_int_eq(amx_var_convert(NULL, &myvar, AMX_VAR_TYPE_ID_STRING), -1);

	amx_var_clean(&myvar);
}
END_TEST

/*
START_TEST (amx_var_type_copy_check)
{
	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &test_var), 0);
	ck_assert_ptr_ne(copyvar.data.data, NULL);
	ck_assert_int_eq(copyvar.type_id, test_var.type_id);
	test_type1_t *test_data = test_var.data.data;
	test_type1_t *copy_data = copyvar.data.data;
	ck_assert_str_eq(copy_data->email, test_data->email);
	ck_assert_str_eq(copy_data->name, test_data->name);
	ck_assert_int_eq(copy_data->age, test_data->age);

	// clean the variants
	amx_var_clean(&copyvar);
}
END_TEST

START_TEST (amx_var_type_copy_invalid_type_check)
{
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);
	myvar.type_id = AMX_VAR_TYPE_ID_CUSTOM_BASE + 100;

	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &myvar), -1);
	ck_assert_ptr_eq(copyvar.data.data, NULL);

	// clean the variants
	amx_var_clean(&myvar);
	amx_var_clean(&copyvar);
}
END_TEST

START_TEST (amx_var_type_copy_no_copy_fn_check)
{
	ck_assert_int_ge(amx_var_register_type(&amx_var_test_type3), AMX_VAR_TYPE_ID_CUSTOM_BASE);
	// initialize a amx variant
	amx_var_t myvar;
	amx_var_init(&myvar);
	myvar.type_id = amx_var_test_type3.type_id;

	// make a copy
	amx_var_t copyvar;
	amx_var_init(&copyvar);

	// do the copy
	ck_assert_int_eq(amx_var_copy(&copyvar, &myvar), -1);
	ck_assert_ptr_eq(copyvar.data.data, NULL);

	// clean the variants
	amx_var_clean(&myvar);
	amx_var_clean(&copyvar);

	amx_var_unregister_type(&amx_var_test_type3);
}
END_TEST
*/
Suite *amx_var_type_suite(void)
{
	Suite *s = suite_create ("amx_variant_types");
	TCase *tc = NULL;

	tc = tcase_create ("amx_var_register_type");
	tcase_add_test (tc, amx_var_register_type_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_register_type_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_register_type_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_unregister_type");
	tcase_add_test (tc, amx_var_unregister_type_null_check);
	tcase_add_test (tc, amx_var_unregister_type_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_type_name");
	tcase_add_test (tc, amx_var_type_name_invalid_type_id_check);
	tcase_add_test (tc, amx_var_type_name_valid_type_id_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_type_id");
	tcase_add_test (tc, amx_var_type_id_invalid_name_check);
	tcase_add_test (tc, amx_var_type_id_valid_name_id_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_set_get_data");
	tcase_add_checked_fixture (tc, amx_var_type_checks_setup, amx_var_type_checks_teardown);
	tcase_add_test (tc, amx_var_set_data_null_check);
	tcase_add_test (tc, amx_var_set_data_move_check);
	tcase_add_test (tc, amx_var_set_data_copy_check);
	tcase_add_test (tc, amx_var_set_data_fixed_types_check);
	tcase_add_test (tc, amx_var_set_data_invalid_type_check);
	tcase_add_test (tc, amx_var_get_data_null_check);
	tcase_add_test (tc, amx_var_get_data_fixed_types_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_type_copy");
	tcase_add_checked_fixture (tc, amx_var_type_checks_setup, amx_var_type_checks_teardown);
	tcase_add_test (tc, amx_var_type_copy_null_check);
	tcase_add_test (tc, amx_var_type_copy_check);
	tcase_add_test (tc, amx_var_type_copy_invalid_type_check);
	tcase_add_test (tc, amx_var_type_copy_no_copy_fn_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_type_convert");
	tcase_add_checked_fixture (tc, amx_var_type_checks_setup, amx_var_type_checks_teardown);
	tcase_add_test (tc, amx_var_type_convert_null_check);
	tcase_add_test (tc, amx_var_type_convert_custom_check);
	//tcase_add_test (tc, amx_var_type_convert_check);
	//tcase_add_test (tc, amx_var_type_convert_invalid_type_check);
	//tcase_add_test (tc, amx_var_type_convert_no_convert_fn_check);
	suite_add_tcase (s, tc);

	return s;
}
