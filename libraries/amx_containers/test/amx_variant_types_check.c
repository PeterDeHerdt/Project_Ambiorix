#include "amx_containers_check.h"
#include <amx_containers/amx_variant.h>

#include "mock_malloc.h"

//static unsigned int test_type_copy = 0;
//static unsigned int test_type_convert = 0;
//static unsigned int test_type_compare = 0;
static unsigned int test_type_delete = 0;

void amx_var_test_type_delete(amx_var_t *var)
{
	(void)var;
	test_type_delete++;
}

static amx_var_type_t amx_var_test_type1 = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = NULL,
	.name = "test_type"
};

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

START_TEST (amx_var_register_type_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_var_register_type(NULL), -1);
	// empty name types must not be accepted
	ck_assert_int_eq (amx_var_register_type(&amx_var_empty_name_type2), -1);
}
END_TEST

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

Suite *amx_var_type_suite(void)
{
	Suite *s = suite_create ("amx_variant_types");
	TCase *tc = NULL;

	tc = tcase_create ("amx_var_register_type");
	tcase_add_test (tc, amx_var_register_type_null_check);
	tcase_add_test (tc, amx_var_register_type_no_memory_check);
	tcase_add_test (tc, amx_var_register_type_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_unregister_type");
	tcase_add_test (tc, amx_var_unregister_type_null_check);
	tcase_add_test (tc, amx_var_unregister_type_check);
	suite_add_tcase (s, tc);

	return s;
}

