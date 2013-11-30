#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_rbuffer.h>

START_TEST (amx_rbuffer_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_new(NULL, 0), -1);
	amx_rbuffer_delete(NULL);
}
END_TEST

START_TEST (amx_rbuffer_new_delete_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 30);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
	ck_assert_ptr_eq (rbuffer, NULL);
}
END_TEST

START_TEST (amx_rbuffer_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_init(NULL, 0), -1);
	amx_rbuffer_clean(NULL);
}
END_TEST

START_TEST (amx_rbuffer_init_clean_check)
{
	amx_rbuffer_t rbuffer;

	ck_assert_int_eq (amx_rbuffer_init(&rbuffer, 30), 0);
	ck_assert_ptr_ne (rbuffer.buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer.buffer_end, rbuffer.buffer_start + 30);
	ck_assert_ptr_eq (rbuffer.read_pos, rbuffer.buffer_start);
	ck_assert_ptr_eq (rbuffer.write_pos, rbuffer.buffer_start);

	amx_rbuffer_clean(&rbuffer);
	ck_assert_ptr_eq (rbuffer.buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer.buffer_end, NULL);
	ck_assert_ptr_eq (rbuffer.read_pos, NULL);
	ck_assert_ptr_eq (rbuffer.write_pos, NULL);
}
END_TEST

START_TEST (amx_rbuffer_grow_shrink_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_grow(NULL, 0), -1);
	ck_assert_int_eq (amx_rbuffer_shrink(NULL, 0), -1);
}
END_TEST

START_TEST (amx_rbuffer_grow_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_grow(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 40);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_grow_read_before_write_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 10;
	rbuffer->write_pos = rbuffer->buffer_start + 20;

	ck_assert_int_eq (amx_rbuffer_grow(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 40);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 10);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 20);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_grow_write_before_read_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 20;
	rbuffer->write_pos = rbuffer->buffer_start + 10;

	ck_assert_int_eq (amx_rbuffer_grow(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 40);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 30);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 20);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 30), -1);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_read_before_write_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 5;
	rbuffer->write_pos = rbuffer->buffer_start + 15;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 20);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_read_before_write_data_loss_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 5;
	rbuffer->write_pos = rbuffer->buffer_start + 25;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 15), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 15);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 15);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_write_before_read_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 25;
	rbuffer->write_pos = rbuffer->buffer_start + 5;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 20);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 15);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_write_before_read_data_loss_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 15;
	rbuffer->write_pos = rbuffer->buffer_start + 5;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 15), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 15);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 15);

	amx_rbuffer_delete(&rbuffer);

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 14;
	rbuffer->write_pos = rbuffer->buffer_start + 5;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 10), 0);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 20);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 4);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 3);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_shrink_full)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 15;
	rbuffer->write_pos = rbuffer->buffer_start + 5;

	ck_assert_int_eq (amx_rbuffer_shrink(rbuffer, 30), 0);
	ck_assert_ptr_eq (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, NULL);
	ck_assert_ptr_eq (rbuffer->read_pos, NULL);
	ck_assert_ptr_eq (rbuffer->write_pos, NULL);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

Suite *amx_rbuffer_suite(void)
{
	Suite *s = suite_create ("amx_rbuffer");
	TCase *tc = NULL;

	tc = tcase_create ("amx_rbuffer_new_init_delete_clean");
	tcase_add_test (tc, amx_rbuffer_new_delete_null_check);
	tcase_add_test (tc, amx_rbuffer_new_delete_check);
	tcase_add_test (tc, amx_rbuffer_init_clean_null_check);
	tcase_add_test (tc, amx_rbuffer_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_grow_shrink");
	tcase_add_test (tc, amx_rbuffer_grow_shrink_null_check);
	tcase_add_test (tc, amx_rbuffer_grow_check);
	tcase_add_test (tc, amx_rbuffer_grow_read_before_write_check);
	tcase_add_test (tc, amx_rbuffer_grow_write_before_read_check);
	tcase_add_test (tc, amx_rbuffer_shrink_check);
	tcase_add_test (tc, amx_rbuffer_shrink_read_before_write_check);
	tcase_add_test (tc, amx_rbuffer_shrink_read_before_write_data_loss_check);
	tcase_add_test (tc, amx_rbuffer_shrink_write_before_read_check);
	tcase_add_test (tc, amx_rbuffer_shrink_write_before_read_data_loss_check);
	tcase_add_test (tc, amx_rbuffer_shrink_full);
	suite_add_tcase (s, tc);

	return s;
}
