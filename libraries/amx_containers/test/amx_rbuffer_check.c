#include <stdio.h>
#include <string.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_rbuffer.h>

#include "mock_malloc.h"

START_TEST (amx_rbuffer_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_new(NULL, 0), -1);
	amx_rbuffer_delete(NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_rbuffer_new_no_memory_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	// second malloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_rbuffer_new(&rbuffer, 30);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (rbuffer, NULL);

	// first malloc fails
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	retval = amx_rbuffer_new(&rbuffer, 30);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (rbuffer, NULL);
}
END_TEST
#endif

START_TEST (amx_rbuffer_new_delete_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_ptr_ne (rbuffer, NULL);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 30);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
	ck_assert_ptr_eq (rbuffer, NULL);

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 0), 0);
	ck_assert_ptr_ne (rbuffer, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, NULL);
	ck_assert_ptr_eq (rbuffer->read_pos, NULL);
	ck_assert_ptr_eq (rbuffer->write_pos, NULL);

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

#ifdef MOCK_MALLOC
START_TEST (amx_rbuffer_grow_no_memory_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	// realloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval =amx_rbuffer_grow(rbuffer, 10);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 30);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST
#endif

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

START_TEST (amx_rbuffer_size_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_size(NULL), 0);
}
END_TEST

START_TEST (amx_rbuffer_size_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_int_eq (amx_rbuffer_size(rbuffer), 0);

	rbuffer->read_pos = rbuffer->buffer_start + 10;
	rbuffer->write_pos = rbuffer->buffer_start + 25;
	ck_assert_int_eq (amx_rbuffer_size(rbuffer), 15);

	rbuffer->read_pos = rbuffer->buffer_start + 20;
	rbuffer->write_pos = rbuffer->buffer_start + 5;
	ck_assert_int_eq (amx_rbuffer_size(rbuffer), 15);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_capacity_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_capacity(NULL), 0);
}
END_TEST

START_TEST (amx_rbuffer_capacity_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_int_eq (amx_rbuffer_capacity(rbuffer), 30);

	amx_rbuffer_grow(rbuffer, 10);
	ck_assert_int_eq (amx_rbuffer_capacity(rbuffer), 40);

	amx_rbuffer_shrink(rbuffer, 20);
	ck_assert_int_eq (amx_rbuffer_capacity(rbuffer), 20);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_is_empty_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_is_empty(NULL), true);
}
END_TEST

START_TEST (amx_rbuffer_is_empty_check)
{
	amx_rbuffer_t *rbuffer = NULL;

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_int_eq (amx_rbuffer_is_empty(rbuffer), true);

	rbuffer->read_pos = rbuffer->buffer_start + 10;
	rbuffer->write_pos = rbuffer->buffer_start + 10;
	ck_assert_int_eq (amx_rbuffer_is_empty(rbuffer), true);

	rbuffer->write_pos = rbuffer->buffer_start + 15;
	ck_assert_int_eq (amx_rbuffer_is_empty(rbuffer), false);

	rbuffer->read_pos = rbuffer->buffer_start + 10;
	rbuffer->write_pos = rbuffer->buffer_start + 5;
	ck_assert_int_eq (amx_rbuffer_is_empty(rbuffer), false);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_write_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_write(NULL, NULL, 0), -1);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_rbuffer_write_no_memory_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 8), 0);

	// realloc fails
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	ssize_t retval = amx_rbuffer_write(rbuffer, data, 10);
	ck_mock_reset(malloc);

	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_ne (rbuffer->buffer_start, NULL);
	ck_assert_ptr_eq (rbuffer->buffer_end, rbuffer->buffer_start + 8);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST
#endif

START_TEST (amx_rbuffer_write_buffer_empty_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 5), 5);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start, data, 5), 0);

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 7), 7);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 12);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start, data, 5), 0);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start + 5, data, 7), 0);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_write_buffer_wrap_buffer_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 25;
	rbuffer->write_pos = rbuffer->buffer_start + 25;
	
	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 25);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start + 25, data, 5), 0);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start, data + 5, 5), 0);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_write_buffer_need_to_grow_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 1;
	rbuffer->write_pos = rbuffer->buffer_start + 0;
	
	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);
	ck_assert_int_eq (amx_rbuffer_capacity(rbuffer), 50);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 21);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);
	ck_assert_int_eq (strncmp(rbuffer->buffer_start, data, 10), 0);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_rbuffer_read(NULL, NULL, 10), -1);

	amx_rbuffer_t *rbuffer = NULL;
	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	ck_assert_int_eq (amx_rbuffer_read(rbuffer, NULL, 10), -1);
	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_buffer_empty_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	char buffer[10] = "";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 5), 0);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_zero_bytes_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";
	char buffer[10] = "";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 0), 0);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";
	char buffer[10] = "";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 5), 5);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 5);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);
	ck_assert_int_eq (strncmp(buffer, data, 5), 0);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 5), 5);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 10);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);
	ck_assert_int_eq (strncmp(buffer, data + 5, 5), 0);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 5), 0);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 10);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 10);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_short_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";
	char buffer[30] = "";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);
	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 30), 20);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 20);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 20);
	ck_assert_int_eq (strncmp(buffer, data, 10), 0);
	ck_assert_int_eq (strncmp(buffer + 10, data, 10), 0);

	rbuffer->read_pos = rbuffer->buffer_start + 15;
	rbuffer->write_pos = rbuffer->buffer_start + 15;
	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);
	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 10), 10);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 25);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);
	ck_assert_int_eq (strncmp(buffer, data, 10), 0);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

START_TEST (amx_rbuffer_read_wrap_check)
{
	amx_rbuffer_t *rbuffer = NULL;
	const char *data = "0123456789";
	char buffer[10] = "";

	ck_assert_int_eq (amx_rbuffer_new(&rbuffer, 30), 0);
	rbuffer->read_pos = rbuffer->buffer_start + 25;
	rbuffer->write_pos = rbuffer->buffer_start + 25;

	ck_assert_int_eq (amx_rbuffer_write(rbuffer, data, 10), 10);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 25);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);

	ck_assert_int_eq (amx_rbuffer_read(rbuffer, buffer, 10), 10);
	ck_assert_ptr_eq (rbuffer->read_pos, rbuffer->buffer_start + 5);
	ck_assert_ptr_eq (rbuffer->write_pos, rbuffer->buffer_start + 5);
	ck_assert_int_eq (strncmp(buffer, data, 10), 0);

	amx_rbuffer_delete(&rbuffer);
}
END_TEST

Suite *amx_rbuffer_suite(void)
{
	Suite *s = suite_create ("amx_rbuffer");
	TCase *tc = NULL;

	tc = tcase_create ("amx_rbuffer_new_init_delete_clean");
	tcase_add_test (tc, amx_rbuffer_new_delete_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_rbuffer_new_no_memory_check);
#endif
	tcase_add_test (tc, amx_rbuffer_new_delete_check);
	tcase_add_test (tc, amx_rbuffer_init_clean_null_check);
	tcase_add_test (tc, amx_rbuffer_init_clean_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_grow_shrink");
	tcase_add_test (tc, amx_rbuffer_grow_shrink_null_check);
	tcase_add_test (tc, amx_rbuffer_grow_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_rbuffer_grow_no_memory_check);
#endif
	tcase_add_test (tc, amx_rbuffer_grow_read_before_write_check);
	tcase_add_test (tc, amx_rbuffer_grow_write_before_read_check);
	tcase_add_test (tc, amx_rbuffer_shrink_check);
	tcase_add_test (tc, amx_rbuffer_shrink_read_before_write_check);
	tcase_add_test (tc, amx_rbuffer_shrink_read_before_write_data_loss_check);
	tcase_add_test (tc, amx_rbuffer_shrink_write_before_read_check);
	tcase_add_test (tc, amx_rbuffer_shrink_write_before_read_data_loss_check);
	tcase_add_test (tc, amx_rbuffer_shrink_full);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_size_capacity");
	tcase_add_test (tc, amx_rbuffer_size_null_check);
	tcase_add_test (tc, amx_rbuffer_size_check);
	tcase_add_test (tc, amx_rbuffer_capacity_null_check);
	tcase_add_test (tc, amx_rbuffer_capacity_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_is_empty");
	tcase_add_test (tc, amx_rbuffer_is_empty_null_check);
	tcase_add_test (tc, amx_rbuffer_is_empty_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_write");
	tcase_add_test (tc, amx_rbuffer_write_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_rbuffer_write_no_memory_check);
#endif
	tcase_add_test (tc, amx_rbuffer_write_buffer_empty_check);
	tcase_add_test (tc, amx_rbuffer_write_buffer_wrap_buffer_check);
	tcase_add_test (tc, amx_rbuffer_write_buffer_need_to_grow_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_rbuffer_read");
	tcase_add_test (tc, amx_rbuffer_read_null_check);
	tcase_add_test (tc, amx_rbuffer_read_buffer_empty_check);
	tcase_add_test (tc, amx_rbuffer_read_zero_bytes_check);
	tcase_add_test (tc, amx_rbuffer_read_check);
	tcase_add_test (tc, amx_rbuffer_read_short_check);
	tcase_add_test (tc, amx_rbuffer_read_wrap_check);
	suite_add_tcase (s, tc);

	return s;
}
