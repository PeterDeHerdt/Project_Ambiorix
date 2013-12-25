#include "amx_containers_check.h"
#include <amx_containers/amx_variant.h>

#include "mock_malloc.h"

static int free_count = 0;

static void amx_var_test_type1_free(amx_var_t *var)
{
	(void)var;
	free_count++;
}

static amx_var_type_t amx_var_test_type1 = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = amx_var_test_type1_free,
	.name = "test_type"
};

START_TEST (amx_var_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_var_new(NULL), -1);
	amx_var_delete(NULL);
}
END_TEST

START_TEST (amx_var_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_var_init(NULL), -1);
	amx_var_clean(NULL);
}
END_TEST

START_TEST (amx_var_new_delete_check)
{
	amx_var_t *var = NULL;

	ck_assert_int_eq (amx_var_new(&var), 0);
	ck_assert_ptr_ne (var, NULL);
	ck_assert_int_eq (var->type_id, 0);
	ck_assert_ptr_eq (var->lit.llist, NULL);
	ck_assert_ptr_eq (var->lit.next, NULL);
	ck_assert_ptr_eq (var->lit.prev, NULL);
	ck_assert_ptr_eq (var->hit.ait, NULL);
	ck_assert_ptr_eq (var->hit.key, NULL);
	ck_assert_ptr_eq (var->hit.next, NULL);

	amx_var_delete(&var);
	ck_assert_ptr_eq (var, NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_var_new_no_memory_check)
{
	amx_var_t *var = NULL;

	// first malloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_var_new(&var);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (var, NULL);
}
END_TEST
#endif

START_TEST (amx_var_new_delete_invalid_type_check)
{
	amx_var_t *var = NULL;

	// register a custom type
	int retval = amx_var_register_type(&amx_var_test_type1);
	ck_assert_int_ge (retval, AMX_VAR_TYPE_ID_CUSTOM_BASE);

	ck_assert_int_eq (amx_var_new(&var), 0);
	// fake type id
	var->type_id = AMX_VAR_TYPE_ID_CUSTOM_BASE + 100;
	amx_var_delete(&var);
	ck_assert_ptr_eq (var, NULL);

	ck_assert_int_eq (amx_var_new(&var), 0);
	// fake type id
	var->type_id = AMX_VAR_TYPE_ID_CUSTOM_BASE + 2;
	amx_var_delete(&var);
	ck_assert_ptr_eq (var, NULL);

	amx_var_unregister_type(&amx_var_test_type1);

}
END_TEST

START_TEST (amx_var_new_delete_custom_type_check)
{
	amx_var_t *var = NULL;

	// register a custom type
	int retval = amx_var_register_type(&amx_var_test_type1);
	ck_assert_int_ge (retval, AMX_VAR_TYPE_ID_CUSTOM_BASE);

	ck_assert_int_eq (amx_var_new(&var), 0);

	free_count = 0;

	char *data = "1234";
	amx_var_set_data_move(var, data, retval);
	amx_var_delete(&var);
	ck_assert_ptr_eq (var, NULL);
	ck_assert_int_eq (free_count, 1);

	amx_var_unregister_type(&amx_var_test_type1);
}
END_TEST

START_TEST (amx_var_get_type_id_null_check)
{
	ck_assert_int_eq (amx_var_get_type_id(NULL), -1);
}
END_TEST

START_TEST (amx_var_get_type_id_check)
{
	amx_var_t *var = NULL;
	ck_assert_int_eq (amx_var_new(&var), 0);
	var->type_id = 1;
	ck_assert_int_eq (amx_var_get_type_id(var), 1);
	amx_var_delete(&var);
}
END_TEST

START_TEST (amx_var_get_type_name_null_check)
{
	ck_assert_ptr_eq ((char *)amx_var_get_type_name(NULL), NULL);
}
END_TEST

START_TEST (amx_var_get_type_name_check)
{
	amx_var_t *var = NULL;
	ck_assert_int_eq (amx_var_new(&var), 0);
	var->type_id = 0;
	ck_assert_str_eq (amx_var_get_type_name(var), AMX_VAR_TYPE_NAME_VOID);
	amx_var_delete(&var);
}
END_TEST

Suite *amx_var_suite(void)
{
	Suite *s = suite_create ("amx_variant");
	TCase *tc = NULL;

	tc = tcase_create ("amx_var_new_init_delete_clean");
	tcase_add_test (tc, amx_var_new_delete_null_check);
	tcase_add_test (tc, amx_var_init_clean_null_check);
	tcase_add_test (tc, amx_var_new_delete_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_var_new_no_memory_check);
#endif
	tcase_add_test (tc, amx_var_new_delete_invalid_type_check);
	tcase_add_test (tc, amx_var_new_delete_custom_type_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_type_id");
	tcase_add_test (tc, amx_var_get_type_id_null_check);
	tcase_add_test (tc, amx_var_get_type_id_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_var_get_type_name");
	tcase_add_test (tc, amx_var_get_type_name_null_check);
	tcase_add_test (tc, amx_var_get_type_name_check);
	suite_add_tcase (s, tc);

	return s;
}
