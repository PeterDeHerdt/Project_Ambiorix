#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_hash.h>
#include <amx_containers/amx_htable.h>

#include "mock_malloc.h"
#include "mock_strdup.h"

static unsigned int deletes = 0;
static amx_htable_t *htable = NULL;
static amx_htable_it_t it[30];

static void amx_htable_setup(void)
{
	ck_assert_int_eq (amx_htable_new(&htable, 0), 0);

	for(int i = 0; i < 30; i++)
	{
		ck_assert_int_eq(amx_htable_it_init(&it[i]), 0);
	}
}

static void amx_htable_teardown(void)
{
	amx_htable_delete(&htable, NULL);
}

static void amx_delete_it_func(const char *key, amx_htable_it_t *it)
{
	ck_assert_ptr_ne (it, NULL);
	ck_assert_ptr_ne ((char *)key, NULL);
	ck_assert_ptr_eq (it->key, NULL);
	ck_assert_ptr_eq (it->ait, NULL);
	ck_assert_ptr_eq (it->next, NULL);
	deletes++;
}

START_TEST (amx_htable_new_delete_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_htable_new(NULL, 0), -1);
	amx_htable_delete(NULL, NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_htable_new_no_memory_check)
{
	amx_htable_t *htable = NULL;

	// first malloc fails
	ck_mock_reset(malloc);
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;

	int retval = amx_htable_new(&htable, 0);
	ck_mock_reset(malloc);

	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (htable, NULL);

	// second malloc fails
	exp = ck_mock_add_expectation(malloc);
	exp->fail = false;
	exp = ck_mock_add_expectation(malloc);
	exp->fail = true;

	retval = amx_htable_new(&htable, 0);
	ck_mock_reset(malloc);
	ck_assert_int_eq (retval, -1);
	ck_assert_ptr_eq (htable, NULL);
}
END_TEST
#endif

START_TEST (amx_htable_new_delete_check)
{
	amx_htable_t *htable = NULL;
	ck_assert_int_eq (amx_htable_new(&htable, 0), 0);
	ck_assert_ptr_ne (htable, NULL);
	ck_assert_ptr_ne (htable->hfunc, NULL);
	ck_assert_ptr_ne (htable->table.buffer, NULL);
	ck_assert_int_eq (htable->items, 0);
	ck_assert_int_eq (amx_array_capacity(&htable->table), 64);
	
	amx_htable_delete(&htable, NULL);
	ck_assert_ptr_eq (htable, NULL);
}
END_TEST

START_TEST (amx_htable_delete_func_check)
{
	char key[10] = "";

	for(int i =0; i < 30; i++)
	{
		sprintf(key, "key%d", i);
		ck_assert_int_eq (amx_htable_insert(htable, key, &it[i]), 0);
	}

	amx_htable_delete(&htable, amx_delete_it_func);
	ck_assert_int_eq (deletes, 30);
}
END_TEST

START_TEST (amx_htable_init_clean_null_check)
{
	// passing NULL pointers should not lead to segfault
	ck_assert_int_eq (amx_htable_init(NULL, 0), -1);
	amx_htable_clean(NULL, NULL);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_htable_init_no_memory_check)
{
	amx_htable_t htable;

	// first malloc fails
	ck_mock_reset(malloc);
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;

	int retval = amx_htable_init(&htable, 20);
	ck_mock_reset(malloc);

	ck_assert_int_eq (retval, -1);
	ck_assert_int_eq (htable.items, 0);
}
END_TEST
#endif

START_TEST (amx_htable_init_clean_check)
{
	amx_htable_t htable;

	ck_assert_int_eq (amx_htable_init(&htable, 0), 0);
	ck_assert_ptr_ne (htable.table.buffer, NULL);
	ck_assert_ptr_ne (htable.hfunc, NULL);
	ck_assert_int_eq (htable.items, 0);
	ck_assert_int_eq (amx_array_capacity(&htable.table), 64);

	amx_htable_clean(&htable, NULL);
	ck_assert_ptr_eq (htable.table.buffer, NULL);
	ck_assert_ptr_ne (htable.hfunc, NULL);
	ck_assert_int_eq (htable.items, 0);
	ck_assert_int_eq (amx_array_capacity(&htable.table), 0);
}
END_TEST

START_TEST (amx_htable_init_reserve_check)
{
	amx_htable_t htable;

	ck_assert_int_eq (amx_htable_init(&htable, 32), 0);
	ck_assert_ptr_ne (htable.table.buffer, NULL);
	ck_assert_ptr_ne (htable.hfunc, NULL);
	ck_assert_int_eq (htable.items, 0);
	ck_assert_int_eq (amx_array_capacity(&htable.table), 32);
	amx_htable_clean(&htable, NULL);

	ck_assert_int_eq (amx_htable_init(&htable, 66), 0);
	ck_assert_ptr_ne (htable.table.buffer, NULL);
	ck_assert_ptr_ne (htable.hfunc, NULL);
	ck_assert_int_eq (htable.items, 0);
	ck_assert_int_eq (amx_array_capacity(&htable.table),66);
	amx_htable_clean(&htable, NULL);
}
END_TEST

START_TEST (amx_htable_set_hash_func_check)
{
	// should not segfault
	amx_htable_set_hash_func(NULL, NULL);

	// check for default hash function
	ck_assert_ptr_eq (htable->hfunc, amx_BKDR_hash);

	// change hash function
	amx_htable_set_hash_func(htable, amx_DJB_hash);
	ck_assert_ptr_eq (htable->hfunc, amx_DJB_hash);

	// reset to default hash function
	amx_htable_set_hash_func(htable, NULL);
	ck_assert_ptr_eq (htable->hfunc, amx_BKDR_hash);
}
END_TEST

START_TEST (amx_htable_key2index_null_check)
{
	// should not segfault
	ck_assert_int_eq (amx_htable_key2index(NULL, NULL), AMX_HTABLE_RANGE);
	ck_assert_int_eq (amx_htable_key2index(NULL, "data"), AMX_HTABLE_RANGE);

	// delete the hash table buffer
	amx_htable_clean(htable, NULL);
	ck_assert_int_eq (amx_htable_key2index(htable, "data"), AMX_HTABLE_RANGE);
}
END_TEST

START_TEST (amx_htable_key2index_check)
{
	// should not segfault
	ck_assert_int_eq (amx_htable_key2index(htable, "Key1"), 18);
	ck_assert_int_eq (amx_htable_key2index(htable, "Key2"), 19);
}
END_TEST

START_TEST (amx_htable_insert_null_check)
{
	ck_assert_int_eq (amx_htable_insert(NULL, NULL, NULL), -1);

	ck_assert_int_eq (amx_htable_insert(htable, NULL, NULL), -1);
	ck_assert_int_eq (htable->items, 0);

	ck_assert_int_eq (amx_htable_insert(htable, "", NULL), -1);
	ck_assert_int_eq (htable->items, 0);

	ck_assert_int_eq (amx_htable_insert(htable, "TestKey", NULL), -1);
	ck_assert_int_eq (htable->items, 0);

	ck_assert_int_eq (amx_htable_insert(htable, NULL, &it[0]), -1);
	ck_assert_int_eq (htable->items, 0);
}
END_TEST

START_TEST (amx_htable_insert_check)
{
	ck_assert_int_eq (amx_htable_insert(htable, "TestKey", &it[0]), 0);
	ck_assert_ptr_ne (it[0].ait, NULL);
	ck_assert_ptr_eq (it[0].ait->array, &htable->table);
	ck_assert_str_eq (it[0].key, "TestKey");
	// it is the first item, so the next pointer should be 0
	ck_assert_ptr_eq (it[0].next, NULL);
	ck_assert_int_eq (htable->items, 1);
}
END_TEST

START_TEST (amx_htable_insert_same_key_check)
{
	ck_assert_int_eq (amx_htable_insert(htable, "TestKey", &it[0]), 0);
	ck_assert_ptr_ne (it[0].ait, NULL);
	ck_assert_ptr_eq (it[0].ait->array, &htable->table);
	ck_assert_str_eq (it[0].key, "TestKey");
	// it is the first item, so the next pointer should be 0
	ck_assert_ptr_eq (it[0].next, NULL);
	ck_assert_int_eq (htable->items, 1);

	ck_assert_int_eq (amx_htable_insert(htable, "TestKey", &it[1]), 0);
	ck_assert_ptr_ne (it[1].ait, NULL);
	ck_assert_ptr_eq (it[1].ait->array, &htable->table);
	ck_assert_str_eq (it[1].key, "TestKey");
	// it is the second item with same key, so the next pointer should be set to it[0]
	ck_assert_ptr_eq (it[1].next, &it[0]);

	// same position
	ck_assert_ptr_eq (it[0].ait, it[1].ait);
	ck_assert_int_eq (htable->items, 2);
}
END_TEST

START_TEST (amx_htable_insert_same_it_check)
{
	ck_assert_int_eq (amx_htable_insert(htable, "TestKey", &it[0]), 0);

	ck_assert_int_eq (amx_htable_insert(htable, "AnotherKey", &it[0]), 0);
	ck_assert_ptr_ne (it[0].ait, NULL);
	ck_assert_str_eq (it[0].key, "AnotherKey");
	ck_assert_int_eq (htable->items, 1);

	ck_assert_int_eq (amx_htable_insert(htable, "AnotherKey", &it[0]), 0);
	ck_assert_ptr_ne (it[0].ait, NULL);
	ck_assert_str_eq (it[0].key, "AnotherKey");
	ck_assert_int_eq (htable->items, 1);
}
END_TEST

START_TEST (amx_htable_insert_grow_check)
{
	amx_htable_delete(&htable, NULL);

	char key[10];
	ck_assert_int_eq (amx_htable_new(&htable, 8), 0);
	for(int i =0; i < 30; i++)
	{
		sprintf(key, "key%d", i);
		ck_assert_int_eq (amx_htable_insert(htable, key, &it[i]), 0);
		ck_assert_ptr_ne (it[i].ait, NULL);
		ck_assert_str_eq (it[i].key, key);
		ck_assert_int_eq (htable->items, i + 1);
		if (i == 6)
		{
			ck_assert_int_eq (amx_htable_capacity(htable), 16);
		}
		if (i == 12)
		{
			ck_assert_int_eq (amx_htable_capacity(htable), 32);
		}
	}

	ck_assert_int_eq (amx_htable_capacity(htable), 64);
}
END_TEST

#ifdef MOCK_MALLOC
START_TEST (amx_htable_insert_grow_no_memory_check)
{
	amx_htable_delete(&htable, NULL);
	ck_assert_int_eq (amx_htable_new(&htable, 8), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key0", &it[0]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key1", &it[1]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key2", &it[2]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key3", &it[3]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key4", &it[4]), 0);

	ck_mock_reset(malloc);
	Expectation_malloc *exp = ck_mock_add_expectation(malloc);
	exp->fail = true;
	int retval = amx_htable_insert(htable, "key5", &it[5]);

	ck_assert_int_eq (retval, -1);
}
END_TEST
#endif

#ifdef MOCK_MALLOC
START_TEST (amx_htable_insert_no_memory_check)
{
	amx_htable_delete(&htable, NULL);
	ck_assert_int_eq (amx_htable_new(&htable, 8), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key0", &it[0]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key1", &it[1]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key2", &it[2]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key3", &it[3]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key4", &it[4]), 0);

	ck_mock_reset(malloc);
	Expectation_strdup *exp = ck_mock_add_expectation(strdup);
	exp->fail = true;
	int retval = amx_htable_insert(htable, "key5", &it[5]);

	ck_assert_int_eq (retval, -1);
}
END_TEST
#endif

START_TEST (amx_htable_is_empty_null_check)
{
	ck_assert_int_eq( amx_htable_is_empty(NULL), true);
}
END_TEST

START_TEST (amx_htable_is_empty_check)
{
	ck_assert_int_eq (amx_htable_is_empty(htable), true);

	amx_htable_insert(htable, "Key1", &it[0]);
	ck_assert_int_eq (amx_htable_is_empty(htable), false);

}
END_TEST

START_TEST (amx_htable_size_null_check)
{
	ck_assert_int_eq( amx_htable_size(NULL), 0);
}
END_TEST

START_TEST (amx_htable_size_check)
{
	ck_assert_int_eq (amx_htable_size(htable), 0);

	amx_htable_insert(htable, "Key1", &it[0]);
	ck_assert_int_eq (amx_htable_size(htable), 1);
	
	amx_htable_insert(htable, "Key1", &it[0]);
	ck_assert_int_eq (amx_htable_size(htable), 1);
	
	amx_htable_insert(htable, "Key2", &it[1]);
	ck_assert_int_eq (amx_htable_size(htable), 2);
}
END_TEST

START_TEST (amx_htable_capacity_null_check)
{
	ck_assert_int_eq( amx_htable_capacity(NULL), 0);
}
END_TEST

START_TEST (amx_htable_capacity_check)
{
	amx_htable_delete(&htable, NULL);
	
	ck_assert_int_eq (amx_htable_new(&htable, 8), 0);
	ck_assert_int_eq (amx_htable_capacity(htable), 8);

	ck_assert_int_eq (amx_htable_insert(htable, "key0", &it[0]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key1", &it[1]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key2", &it[2]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key3", &it[3]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key4", &it[4]), 0);
	ck_assert_int_eq (amx_htable_insert(htable, "key5", &it[5]), 0);
	ck_assert_int_eq (amx_htable_capacity(htable), 16);
}
END_TEST

START_TEST (amx_htable_get_null_check)
{
	ck_assert_ptr_eq (amx_htable_get(NULL, NULL), NULL);
	
	ck_assert_ptr_eq (amx_htable_get(htable, NULL), NULL);
	ck_assert_ptr_eq (amx_htable_get(htable, ""), NULL);
}
END_TEST

START_TEST (amx_htable_get_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);
	amx_htable_insert(htable, "Key3", &it[2]);
	amx_htable_insert(htable, "Key4", &it[3]);
	amx_htable_insert(htable, "Key5", &it[4]);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key3"), &it[2]);
	ck_assert_str_eq (it[2].key, "Key3");
	ck_assert_ptr_ne (it[2].ait, NULL);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key1"), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_ne (it[0].ait, NULL);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key4"), &it[3]);
	ck_assert_str_eq (it[3].key, "Key4");
	ck_assert_ptr_ne (it[3].ait, NULL);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key2"), &it[1]);
	ck_assert_str_eq (it[1].key, "Key2");
	ck_assert_ptr_ne (it[1].ait, NULL);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key5"), &it[4]);
	ck_assert_str_eq (it[4].key, "Key5");
	ck_assert_ptr_ne (it[4].ait, NULL);

	ck_assert_ptr_eq (amx_htable_get(htable, "Dummy"), NULL);

	// clean the table
	amx_htable_clean(htable, NULL);
	ck_assert_ptr_eq (amx_htable_get(htable, "Dummy"), NULL);
}
END_TEST

START_TEST (amx_htable_get_chained_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key610", &it[1]);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key610"), &it[1]);
	ck_assert_str_eq (it[1].key, "Key610");
	ck_assert_ptr_ne (it[1].ait, NULL);
	ck_assert_ptr_eq (it[1].next, &it[0]);

	ck_assert_ptr_eq (amx_htable_get(htable, "Key1"), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_ne (it[0].ait, NULL);
}
END_TEST

START_TEST (amx_htable_get_first_null_check)
{
	ck_assert_ptr_eq (amx_htable_get_first(NULL), NULL);

	ck_assert_ptr_eq (amx_htable_get_first(htable), NULL);
}
END_TEST

START_TEST (amx_htable_get_first_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);

	ck_assert_ptr_eq (amx_htable_get_first(htable), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_ne (it[0].ait, NULL);

	// clean the table
	amx_htable_clean(htable, NULL);
	ck_assert_ptr_eq (amx_htable_get_first(htable), NULL);
}
END_TEST

START_TEST (amx_htable_take_null_check)
{
	ck_assert_ptr_eq (amx_htable_take(NULL, NULL), NULL);

	ck_assert_ptr_eq (amx_htable_take(htable, NULL), NULL);
	ck_assert_ptr_eq (amx_htable_take(htable, ""), NULL);
}
END_TEST

START_TEST (amx_htable_take_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);
	amx_htable_insert(htable, "Key3", &it[2]);
	amx_htable_insert(htable, "Key4", &it[3]);
	amx_htable_insert(htable, "Key5", &it[4]);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key3"), &it[2]);
	ck_assert_str_eq (it[2].key, "Key3");
	ck_assert_ptr_eq (it[2].ait, NULL);
	ck_assert_int_eq (htable->items, 4);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key1"), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_eq (it[0].ait, NULL);
	ck_assert_int_eq (htable->items, 3);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key4"), &it[3]);
	ck_assert_str_eq (it[3].key, "Key4");
	ck_assert_ptr_eq (it[3].ait, NULL);
	ck_assert_int_eq (htable->items, 2);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key2"), &it[1]);
	ck_assert_str_eq (it[1].key, "Key2");
	ck_assert_ptr_eq (it[1].ait, NULL);
	ck_assert_int_eq (htable->items, 1);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key5"), &it[4]);
	ck_assert_str_eq (it[4].key, "Key5");
	ck_assert_ptr_eq (it[4].ait, NULL);
	ck_assert_int_eq (htable->items, 0);

	amx_htable_it_clean(&it[0], NULL);
	amx_htable_it_clean(&it[1], NULL);
	amx_htable_it_clean(&it[2], NULL);
	amx_htable_it_clean(&it[3], NULL);
	amx_htable_it_clean(&it[4], NULL);
}
END_TEST

START_TEST (amx_htable_take_chained_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key610", &it[1]);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key610"), &it[1]);
	ck_assert_str_eq (it[1].key, "Key610");
	ck_assert_ptr_eq (it[1].ait, NULL);
	ck_assert_ptr_eq (it[1].next, NULL);
	ck_assert_int_eq (htable->items, 1);

	ck_assert_ptr_eq (amx_htable_take(htable, "Key1"), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_eq (it[0].ait, NULL);
	ck_assert_int_eq (htable->items, 0);

	ck_assert_ptr_eq (amx_htable_take(htable, "Dummy"), NULL);

	amx_htable_it_clean(&it[0], NULL);
	amx_htable_it_clean(&it[1], NULL);
}
END_TEST

START_TEST (amx_htable_take_first_null_check)
{
	ck_assert_ptr_eq (amx_htable_take_first(NULL), NULL);

	ck_assert_ptr_eq (amx_htable_take_first(htable), NULL);
}
END_TEST

START_TEST (amx_htable_take_first_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);

	ck_assert_ptr_eq (amx_htable_take_first(htable), &it[0]);
	ck_assert_str_eq (it[0].key, "Key1");
	ck_assert_ptr_eq (it[0].ait, NULL);

	amx_htable_it_clean(&it[0], NULL);

	// clean the table
	amx_htable_clean(htable, NULL);
	ck_assert_ptr_eq (amx_htable_take_first(htable), NULL);
}
END_TEST

START_TEST (amx_htable_contains_null_check)
{
	ck_assert_int_eq (amx_htable_contains(NULL, NULL), false);

	ck_assert_int_eq (amx_htable_contains(htable, NULL), false);
	ck_assert_int_eq (amx_htable_contains(htable, ""), false);
}
END_TEST

START_TEST (amx_htable_contains_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);

	ck_assert_int_eq (amx_htable_contains(htable, "Key1"), true);
	ck_assert_int_eq (amx_htable_contains(htable, "Key2"), true);
	ck_assert_int_eq (amx_htable_contains(htable, "Key3"), false);
}
END_TEST

START_TEST (amx_htable_it_get_next_null_check)
{
	ck_assert_ptr_eq (amx_htable_it_get_next(NULL), NULL);
	ck_assert_ptr_eq (amx_htable_it_get_next(&it[0]), NULL);
}
END_TEST

START_TEST (amx_htable_it_get_next_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);
	amx_htable_insert(htable, "Key3", &it[2]);
	amx_htable_insert(htable, "Key4", &it[3]);
	amx_htable_insert(htable, "Key5", &it[4]);

	unsigned int count = 0;
	amx_htable_it_t *it = amx_htable_get_first(htable);
	while(it)
	{
		it = amx_htable_it_get_next(it);
		count++;
	}
	
	ck_assert_int_eq (count, 5);
}
END_TEST

START_TEST (amx_htable_it_get_next_chained_check)
{
	amx_htable_insert(htable, "Key", &it[0]);
	amx_htable_insert(htable, "Key", &it[1]);
	amx_htable_insert(htable, "Key", &it[2]);
	amx_htable_insert(htable, "Key", &it[3]);
	amx_htable_insert(htable, "Key", &it[4]);

	amx_array_it_t *ait = it[0].ait;
	unsigned int count = 0;
	amx_htable_it_t *it = amx_htable_get_first(htable);
	while(it)
	{
		ck_assert_ptr_eq(it->ait, ait);
		it = amx_htable_it_get_next(it);
		count++;
	}
	
	ck_assert_int_eq (count, 5);
}
END_TEST

START_TEST (amx_htable_it_get_next_key_null_check)
{
	ck_assert_ptr_eq (amx_htable_it_get_next_key(NULL), NULL);
	ck_assert_ptr_eq (amx_htable_it_get_next_key(&it[0]), NULL);
}
END_TEST

START_TEST (amx_htable_it_get_next_key_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);
	amx_htable_insert(htable, "Key3", &it[2]);
	amx_htable_insert(htable, "Key4", &it[3]);
	amx_htable_insert(htable, "Key5", &it[4]);
	amx_htable_insert(htable, "Key610", &it[11]);
	amx_htable_insert(htable, "Key1", &it[5]);
	amx_htable_insert(htable, "Key2", &it[6]);
	amx_htable_insert(htable, "Key3", &it[7]);
	amx_htable_insert(htable, "Key1", &it[8]);
	amx_htable_insert(htable, "Key2", &it[9]);
	amx_htable_insert(htable, "Key3", &it[10]);

	unsigned int count = 0;
	amx_array_it_t *ait = it[0].ait;
	amx_htable_it_t *iter = amx_htable_get(htable, "Key1");
	while(iter)
	{
		ck_assert_ptr_eq (iter->ait, ait);
		iter = amx_htable_it_get_next_key(iter);
		count++;
	}
	
	ck_assert_int_eq (count, 3);

	count = 0;
	ait = it[2].ait;
	iter = amx_htable_get(htable, "Key3");
	while(iter)
	{
		ck_assert_ptr_eq (iter->ait, ait);
		iter = amx_htable_it_get_next_key(iter);
		count++;
	}

	ck_assert_int_eq (count, 3);

	count = 0;
	iter = amx_htable_get(htable, "Key4");
	while(iter)
	{
		iter = amx_htable_it_get_next_key(iter);
		count++;
	}
	
	ck_assert_int_eq (count, 1);
}
END_TEST

START_TEST (amx_htable_it_take_null_check)
{
	amx_htable_it_take(NULL);
	amx_htable_it_take(&it[0]);
}
END_TEST

START_TEST (amx_htable_it_take_check)
{
	amx_htable_insert(htable, "Key1", &it[0]);
	amx_htable_insert(htable, "Key2", &it[1]);
	amx_htable_insert(htable, "Key3", &it[2]);
	amx_htable_insert(htable, "Key4", &it[3]);
	amx_htable_insert(htable, "Key5", &it[4]);

	amx_htable_it_take(&it[3]);
	ck_assert_int_eq (amx_htable_size(htable), 4);
	ck_assert_str_eq (it[3].key, "Key4");
	ck_assert_ptr_eq (it[3].ait, NULL);
	ck_assert_ptr_eq (it[3].next, NULL);

	amx_htable_it_take(&it[4]);
	ck_assert_int_eq (amx_htable_size(htable), 3);
	ck_assert_str_eq (it[4].key, "Key5");
	ck_assert_ptr_eq (it[4].ait, NULL);
	ck_assert_ptr_eq (it[4].next, NULL);

	amx_htable_it_clean(&it[3], NULL);
	amx_htable_it_clean(&it[4], NULL);
}
END_TEST

START_TEST (amx_htable_it_take_chained_check)
{
	amx_htable_insert(htable, "Key", &it[0]);
	amx_htable_insert(htable, "Key", &it[1]);
	amx_htable_insert(htable, "Key", &it[2]);
	amx_htable_insert(htable, "Key", &it[3]);
	amx_htable_insert(htable, "Key", &it[4]);

	amx_htable_it_take(&it[2]);
	ck_assert_int_eq (amx_htable_size(htable), 4);
	ck_assert_str_eq (it[2].key, "Key");
	ck_assert_ptr_eq (it[2].ait, NULL);
	ck_assert_ptr_eq (it[2].next, NULL);

	amx_htable_it_take(&it[0]);
	ck_assert_int_eq( amx_htable_size(htable), 3);
	ck_assert_str_eq (it[0].key, "Key");
	ck_assert_ptr_eq (it[0].ait, NULL);
	ck_assert_ptr_eq (it[0].next, NULL);
	
	amx_htable_it_take(&it[4]);
	ck_assert_int_eq( amx_htable_size(htable), 2);
	ck_assert_str_eq (it[4].key, "Key");
	ck_assert_ptr_eq (it[4].ait, NULL);
	ck_assert_ptr_eq (it[4].next, NULL);

	amx_htable_it_clean(&it[0], NULL);
	amx_htable_it_clean(&it[2], NULL);
	amx_htable_it_clean(&it[4], NULL);
}
END_TEST

START_TEST (amx_htable_it_get_key_check)
{
	ck_assert_ptr_eq ((void *)amx_htable_it_get_key(NULL), NULL);
	ck_assert_ptr_eq ((void *)amx_htable_it_get_key(&it[0]), NULL);

	amx_htable_insert(htable, "SomeKey", &it[0]);
	ck_assert_str_eq (amx_htable_it_get_key(&it[0]), "SomeKey");
	amx_htable_take(htable, "SomeKey");
	ck_assert_str_eq (amx_htable_it_get_key(&it[0]), "SomeKey");

	amx_htable_it_clean(&it[0], NULL);
}
END_TEST

START_TEST (amx_htable_it_init_null_check)
{
	amx_htable_it_init(NULL);
}
END_TEST

START_TEST (amx_htable_it_clean_null_check)
{
	amx_htable_it_clean(NULL, NULL);
}
END_TEST

START_TEST (amx_htable_it_clean_func_check)
{
	deletes = 0;
	amx_htable_insert(htable, "SomeKey", &it[0]);
	ck_assert_str_eq(it[0].key, "SomeKey");
	amx_htable_it_clean(&it[0], amx_delete_it_func);
	ck_assert_int_eq (deletes, 1);
}
END_TEST

Suite *amx_htable_suite(void)
{
	Suite *s = suite_create ("amx_htable");
	TCase *tc = NULL;

	tc = tcase_create ("amx_htable_new_init_delete_clean");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_new_delete_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_htable_new_no_memory_check);
#endif
	tcase_add_test (tc, amx_htable_delete_func_check);
	tcase_add_test (tc, amx_htable_new_delete_check);
	tcase_add_test (tc, amx_htable_init_clean_null_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_htable_init_no_memory_check);
#endif
	tcase_add_test (tc, amx_htable_init_clean_check);
	tcase_add_test (tc, amx_htable_init_reserve_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_hash_func");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_set_hash_func_check);
	tcase_add_test (tc, amx_htable_key2index_null_check);
	tcase_add_test (tc, amx_htable_key2index_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_insert");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_insert_null_check);
	tcase_add_test (tc, amx_htable_insert_check);
	tcase_add_test (tc, amx_htable_insert_same_key_check);
	tcase_add_test (tc, amx_htable_insert_same_it_check);
	tcase_add_test (tc, amx_htable_insert_grow_check);
#ifdef MOCK_MALLOC
	tcase_add_test (tc, amx_htable_insert_grow_no_memory_check);
	tcase_add_test (tc, amx_htable_insert_no_memory_check);
#endif
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_is_empty");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_is_empty_null_check);
	tcase_add_test (tc, amx_htable_is_empty_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_size");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_size_null_check);
	tcase_add_test (tc, amx_htable_size_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_capacity");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_capacity_null_check);
	tcase_add_test (tc, amx_htable_capacity_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_get");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_get_null_check);
	tcase_add_test (tc, amx_htable_get_check);
	tcase_add_test (tc, amx_htable_get_chained_check);
	tcase_add_test (tc, amx_htable_get_first_null_check);
	tcase_add_test (tc, amx_htable_get_first_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_take");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_take_null_check);
	tcase_add_test (tc, amx_htable_take_check);
	tcase_add_test (tc, amx_htable_take_chained_check);
	tcase_add_test (tc, amx_htable_take_first_null_check);
	tcase_add_test (tc, amx_htable_take_first_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_contains");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_contains_null_check);
	tcase_add_test (tc, amx_htable_contains_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_it_get_next");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_it_get_next_null_check);
	tcase_add_test (tc, amx_htable_it_get_next_check);
	tcase_add_test (tc, amx_htable_it_get_next_chained_check);
	tcase_add_test (tc, amx_htable_it_get_next_key_null_check);
	tcase_add_test (tc, amx_htable_it_get_next_key_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_it_take");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_it_take_null_check);
	tcase_add_test (tc, amx_htable_it_take_check);
	tcase_add_test (tc, amx_htable_it_take_chained_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_it_get_key");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_it_get_key_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_htable_it_init_clean");
	tcase_add_checked_fixture (tc, amx_htable_setup, amx_htable_teardown);
	tcase_add_test (tc, amx_htable_it_init_null_check);
	tcase_add_test (tc, amx_htable_it_clean_null_check);
	tcase_add_test (tc, amx_htable_it_clean_func_check);
	suite_add_tcase (s, tc);

	return s;
}
