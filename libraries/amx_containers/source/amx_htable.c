/****************************************************************************
**
** - DISCLAIMER OF WARRANTY -
**
** THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE SOURCE
** CODE IS WITH YOU. SHOULD THE SOURCE CODE PROVE DEFECTIVE, YOU
** ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
**
** - LIMITATION OF LIABILITY -
**
** IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
** WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES
** AND/OR DISTRIBUTES THE SOURCE CODE, BE LIABLE TO YOU FOR DAMAGES,
** INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
** ARISING OUT OF THE USE OR INABILITY TO USE THE SOURCE CODE
** (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED
** INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE
** OF THE SOURCE CODE TO OPERATE WITH ANY OTHER PROGRAM), EVEN IF SUCH
** HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGES.
**
****************************************************************************/

#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <amx_containers/amx_hash.h>
#include <amx_containers/amx_htable.h>

/**
 @file
 @brief
 Ambiorix hash table API implementation
*/

static int amx_htable_grow(amx_htable_t *htable)
{
	int retval = -1;
	size_t capacity = htable->items;
	
	amx_array_t temp;
	retval = amx_array_init(&temp, capacity);
	if (retval == -1)
	{
		goto exit;
	}

	// move the items to the temp array and reset the table
	amx_array_it_t *it = amx_array_get_first(&htable->table);
	while(it)
	{
		amx_htable_it_t *hit = amx_array_it_take_data(it);
		hit->ait = NULL;
		amx_array_append_data(&temp, hit);
		while(hit->next)
		{
			amx_htable_it_t *hit_next = hit->next;
			hit->next = NULL;
			hit = hit_next;
			hit->ait = NULL;
			amx_array_append_data(&temp, hit);
		}
		it = amx_array_get_first(&htable->table);
	}
	htable->items = 0;

	// grow
	capacity = htable->table.items;
	amx_array_grow(&htable->table, capacity>1024?1024:capacity);

	// add the items back to the htable
	it = amx_array_get_first(&temp);
	while(it)
	{
		amx_htable_it_t *hit = amx_array_it_take_data(it);
		amx_htable_insert(htable, hit->key, hit);
		it = amx_array_get_first(&temp);
	}

	// remove temp array
	amx_array_clean(&temp, NULL);

exit:
	return retval;
}

static void amx_htable_it_delete_func(amx_array_it_t *it)
{
	amx_htable_t *htable = (amx_htable_t *)it->array;

	amx_htable_it_t *current = it->data;
	amx_htable_it_t *next = NULL;
	char *key = NULL;
	while(current)
	{
		next = current->next;
		key = current->key;
		current->key = NULL;
		amx_htable_it_take(current);
		if (htable->it_del)
		{
			htable->it_del(key, current);
		}
		free(key);
		current = next;
	}

	return;
}

int amx_htable_new(amx_htable_t **htable, size_t reserve)
{
	int retval = -1;
	if (!htable)
	{
		goto exit;
	}

	*htable = calloc(1, sizeof(amx_htable_t));
	if (!(*htable))
	{
		goto exit;
	}

	retval = amx_htable_init(*htable, reserve);
	if (retval == -1)
	{
		free(*htable);
		*htable = NULL;
	}

exit:
	return retval;
}

void amx_htable_delete(amx_htable_t **htable, amx_htable_it_delete_t func)
{
	if (!htable || !(*htable))
	{
		goto exit;
	}

	(*htable)->it_del = func;
	amx_array_clean(&(*htable)->table, amx_htable_it_delete_func);
	free(*htable);
	*htable = NULL;

exit:
	return;
}

int amx_htable_init(amx_htable_t *htable, size_t reserve)
{
	int retval = -1;
	if (!htable)
	{
		goto exit;
	}

	htable->items = 0;
	htable->hfunc = amx_BKDR_hash;

	if (amx_array_init(&htable->table, reserve?reserve:64) == -1)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

void amx_htable_clean(amx_htable_t *htable, amx_htable_it_delete_t func)
{
	if (!htable)
	{
		goto exit;
	}

	htable->it_del = func;
	amx_array_clean(&htable->table, amx_htable_it_delete_func);
	htable->items = 0;

exit:
	return;
}

void amx_htable_set_hash_func(amx_htable_t *htable, amx_htable_hash_func_t func)
{
	if (!htable)
	{
		goto exit;
	}

	if (func)
	{
		htable->hfunc = func;
	}
	else
	{
		htable->hfunc = amx_BKDR_hash;
	}

exit:
	return;
}

unsigned int amx_htable_key2index(const amx_htable_t *htable, const char *key)
{
	unsigned int hash = AMX_HTABLE_RANGE;
	if (!htable || !htable->table.items)
	{
		goto exit;
	}

	hash = htable->hfunc(key, strlen(key)) % htable->table.items;

exit:
	return hash;
}

int amx_htable_insert(amx_htable_t *htable, const char *key, amx_htable_it_t *it)
{
	int retval = -1;
	if (!htable || !key || !(*key) || !it)
	{
		goto exit;
	}

	// remove the iterator first
	amx_htable_it_take(it);
	if (it->key && strcmp(it->key, key) != 0)
	{
		free(it->key);
		it->key = NULL;
	}

	if (((htable->items + 1) * 100) / htable->table.items >= 75)
	{
		// time to grow the table
		if (amx_htable_grow(htable) == -1)
		{
			goto exit;
		}
	}

	// update htable iterator
	if (!it->key)
	{
		it->key = strdup(key);
		if (!it->key)
		{
			goto exit;
		}
	}

	unsigned int index = amx_htable_key2index(htable, key);
	amx_array_it_t *ait = amx_array_get_at(&htable->table, index);
	// insert item
	if (ait->data)
	{
		it->next = ait->data;
	}
	amx_array_it_set_data(ait, it);
	it->ait = ait;
	htable->items++;

	retval = 0;

exit:
	return retval;
}

amx_htable_it_t *amx_htable_get(const amx_htable_t *htable, const char *key)
{
	amx_htable_it_t *it = NULL;
	if (!htable || !htable->table.items || !key || !(*key))
	{
		goto exit;
	}

	unsigned int index = amx_htable_key2index(htable, key);
	amx_array_it_t *ait = amx_array_get_at(&htable->table, index);

	it = ait->data;
	while(it && strcmp(key, it->key) != 0)
	{
		it = it->next;
	}

exit:
	return it;
}

amx_htable_it_t *amx_htable_take(amx_htable_t *htable, const char *key)
{
	amx_htable_it_t *it = amx_htable_get(htable, key);
	if (it)
	{
		amx_htable_it_take(it);
	}
	return it;
}

amx_htable_it_t *amx_htable_get_first(const amx_htable_t *htable)
{
	amx_htable_it_t *it = NULL;
	if (!htable)
	{
		goto exit;
	}

	amx_array_it_t *ait = amx_array_get_first(&htable->table);
	if (!ait)
	{
		goto exit;
	}
	it = ait->data;

exit:
	return it;
}
