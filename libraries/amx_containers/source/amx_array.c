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

#include <stdlib.h>
#include <string.h>

#include <amx_containers/amx_array.h>

#define AMX_ARRAY_AUTO_GROW_ITEMS 3

/**
 @file
 @brief
 Ambiorix array API implementation
*/

static void amx_array_initialize_items(amx_array_t *array, unsigned int start_pos)
{
	amx_array_it_t *it = NULL;

	for (unsigned int index = start_pos; index < array->items; index++)
	{
		it = &(array->buffer[index]);
		it->array = array;
		it->data = NULL;
	}
}

static void amx_array_clean_items(amx_array_t *array, size_t start_pos, size_t items, amx_array_it_delete_t func)
{
	amx_array_it_t *it = NULL;
	for (unsigned int index = start_pos; index < start_pos + items; index++)
	{
		it = &(array->buffer[index]);
		if (it->data)
		{
			if (func)
			{
				func(it);
			}
			it->data = NULL;
		}
	}

	return;
}

static int amx_array_realloc(amx_array_t *array, size_t items)
{
	int retval = -1;
	amx_array_it_t *buffer = NULL;
	
	if (array->buffer)
	{
		buffer = realloc(array->buffer, sizeof(amx_array_it_t) * items);
	}
	else
	{
		buffer = calloc(items, sizeof(amx_array_it_t));
	}
	if (buffer)
	{
		array->buffer = buffer;
		array->items = items;
		retval = 0;
	}

	return retval;
}

static size_t amx_array_calculate_last_used(amx_array_t *array, size_t start)
{
	size_t index =start;
	while(index > 0 && !array->buffer[index].data)
	{
		index--;
	}

	return index;
}

static size_t amx_array_calculate_first_used(amx_array_t *array, size_t start)
{
	size_t index = start;
	while(index < array->items && !array->buffer[index].data)
	{
		index++;
	}

	return (index == array->items)?0:index;
}

int amx_array_new(amx_array_t **array, size_t items)
{
	int retval = -1;
	if (!array)
	{
		goto exit;
	}

	/* allocate the array structure */
	*array = calloc(1, sizeof(amx_array_t));
	if (!(*array))
	{
		goto exit;
	}

	/* set the number of items in the array */
	(*array)->items = items;
	(*array)->first_used = 0;
	(*array)->last_used = 0;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	/* allocate the buffer */
	amx_array_realloc(*array, items);
	if (!(*array)->buffer)
	{
		free(*array);
		*array = NULL;
		goto exit;
	}
	amx_array_initialize_items(*array, 0);

	retval = 0;

exit:
	return retval;
}

void amx_array_delete(amx_array_t **array, amx_array_it_delete_t func)
{
	if (!array)
	{
		goto exit;
	}

	if (func)
	{
		amx_array_clean_items(*array, 0, (*array)->items, func);
	}

	free((*array)->buffer);
	free(*array);
	*array = NULL;

exit:
	return;
}

int amx_array_init(amx_array_t *array, size_t items)
{
	int retval = -1;
	if (!array)
	{
		goto exit;
	}

	// initialize array data
	array->items = 0;
	array->buffer = NULL;
	array->first_used = 0;
	array->last_used = 0;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	// allocate the buffer
	amx_array_realloc(array, items);
	if (!array->buffer)
	{
		goto exit;
	}

	// set the allocated size
	array->items = items;
	amx_array_initialize_items(array, 0);

	retval = 0;

exit:
	return retval;
}

void amx_array_clean(amx_array_t *array, amx_array_it_delete_t func)
{
	if (!array)
	{
		goto exit;
	}

	if (func)
	{
		amx_array_clean_items(array, 0, array->items, func);
	}

	free(array->buffer);
	array->buffer = NULL;
	array->items = 0;
	array->first_used = 0;
	array->last_used = 0;

exit:
	return;
}

int amx_array_grow(amx_array_t *array, size_t items)
{
	int retval = -1;
	if (!array)
	{
		goto exit;
	}

	if (items == 0)
	{
		retval = 0;
		goto exit;
	}

	size_t old_items = array->items;
	retval = amx_array_realloc(array, array->items + items);
	amx_array_initialize_items(array, old_items);

exit:
	return retval;
}

int amx_array_shrink(amx_array_t *array, size_t items, amx_array_it_delete_t func)
{
	int retval = -1;
	if (!array)
	{
		goto exit;
	}

	if (items > array->items)
	{
		/* out of range */
		goto exit;
	}

	if (items == array->items)
	{
		amx_array_clean(array, func);
		
		retval = 0;
		goto exit;
	}

	amx_array_clean_items(array, array->items - items, items, func);
	retval = amx_array_realloc(array, array->items - items);
	array->last_used = amx_array_calculate_last_used(array, array->items - 1);
	if (array->first_used > array->items - 1)
	{
		array->first_used = 0;
	}

exit:
	return retval;
}

int amx_array_shift_right(amx_array_t *array, size_t items, amx_array_it_delete_t func)
{
	int retval = -1;
	if (!array || items > array->items)
	{
		goto exit;
	}

	if (!items)
	{
		retval = 0;
		goto exit;
	}

	if (items == array->items)
	{
		amx_array_clean_items(array, 0, array->items, func);
		array->last_used = 0;
		array->first_used = 0;
		retval = 0;
		goto exit;
	}

	amx_array_it_t *src = array->buffer;
	amx_array_it_t *dst = &array->buffer[items];
	size_t len = (array->items - items) * sizeof(amx_array_it_t);

	memmove(dst, src, len);
	amx_array_clean_items(array, 0, items, func);
	array->first_used = amx_array_calculate_first_used(array, items);
	array->last_used = amx_array_calculate_last_used(array, array->items - 1);

	retval = 0;

exit:
	return retval;
}

int amx_array_shift_left(amx_array_t *array, size_t items,  amx_array_it_delete_t func)
{
	int retval = -1;
	if (!array || items > array->items)
	{
		goto exit;
	}

	if (!items)
	{
		retval = 0;
		goto exit;
	}

	if (items == array->items)
	{
		amx_array_clean_items(array, 0, array->items, func);
		retval = 0;
		array->last_used = 0;
		array->first_used = 0;
		goto exit;
	}

	amx_array_it_t *src = &array->buffer[items];
	amx_array_it_t *dst = array->buffer;
	size_t len = (array->items - items) * sizeof(amx_array_it_t);

	memmove(dst, src, len);
	amx_array_clean_items(array, array->items - items, items, func);
	array->first_used = amx_array_calculate_first_used(array, 0);
	array->last_used = amx_array_calculate_last_used(array, array->items - items);

	retval = 0;

exit:
	return retval;
}

bool amx_array_is_empty(const amx_array_t *array)
{
	bool retval = true;
	if (!array)
	{
		goto exit;
	}

	if (array->last_used != 0)
	{
		retval = false;
	}

	if (array->buffer && array->buffer[0].data)
	{
		retval = false;
	}

exit:
	return retval;
}

size_t amx_array_size(const amx_array_t *array)
{
	size_t retval = 0;
	if (!array)
	{
		goto exit;
	}

	for(size_t index = 0; index < array->items; index++)
	{
		if (array->buffer[index].data)
		{
			retval++;
		}
	}

exit:
	return retval;
}

amx_array_it_t *amx_array_append_data(amx_array_t *array, void *data)
{
	amx_array_it_t *it = NULL;
	if (!array || !data)
	{
		goto exit;
	}

	size_t index = 0;
	if (!amx_array_is_empty(array))
	{
		index = array->last_used + 1;
	}

	if (index >= array->items)
	{
		if (amx_array_grow(array, AMX_ARRAY_AUTO_GROW_ITEMS) == -1)
		{
			goto exit;
		}
	}

	it = amx_array_set_data_at(array, index, data);

exit:
	return it;
}

amx_array_it_t *amx_array_prepend_data(amx_array_t *array, void *data)
{
	amx_array_it_t *it = NULL;
	if (!array || !data)
	{
		goto exit;
	}

	size_t index = 0;
	bool grow = ((!amx_array_is_empty(array) && array->first_used == 0) || !array->buffer);

	if (grow)
	{
		if (amx_array_grow(array, AMX_ARRAY_AUTO_GROW_ITEMS) == -1)
		{
			goto exit;
		}
		amx_array_shift_right(array, AMX_ARRAY_AUTO_GROW_ITEMS, NULL);
	}

	if (!amx_array_is_empty(array))
	{
		index = array->first_used - 1;
	}

	it = amx_array_set_data_at(array, index, data);

exit:
	return it;
}

amx_array_it_t *amx_array_set_data_at(amx_array_t *array, unsigned int index, void *data)
{
	amx_array_it_t *it = amx_array_get_at(array, index);
	if (!it)
	{
		goto exit;
	}

	if (data)
	{
		if (amx_array_is_empty(array))
		{
			array->last_used = index;
			array->first_used = index;
		}
		else 
		{
			array->last_used = (array->last_used < index)?index:array->last_used;
			array->first_used = (array->first_used > index)?index:array->first_used;
		}
		it->data = data;
	} 
	else
	{
		void *tmp = it->data;
		it->data = data;
		if (tmp && index == array->last_used)
		{
			array->last_used = amx_array_calculate_last_used(array, array->last_used);
		}
		if (tmp && index == array->first_used)
		{
			array->first_used = amx_array_calculate_first_used(array, array->first_used);
		}
	}

exit:
	return it;
}

amx_array_it_t *amx_array_get_at(const amx_array_t *array, unsigned int index)
{
	amx_array_it_t *it = NULL;
	if (!array || !array->buffer)
	{
		goto exit;
	}

	if (index >= array->items)
	{
		goto exit;
	}

	it = &array->buffer[index];

exit:
	return it;
}

amx_array_it_t *amx_array_get_first(const amx_array_t *array)
{
	amx_array_it_t *it = NULL;
	if (!array)
	{
		goto exit;
	}

	if (!amx_array_is_empty(array))
	{
		it = &array->buffer[array->first_used];
	}

exit:
	return it;
}

amx_array_it_t *amx_array_get_first_free(const amx_array_t *array)
{
	amx_array_it_t *it = NULL;
	if (!array)
	{
		goto exit;
	}

	size_t index = 0;
	while(index < array->items && array->buffer[index].data != NULL)
	{
		index++;
	}

	if (index < array->items)
	{
		it = &array->buffer[index];
	}

exit:
	return it;
}

amx_array_it_t *amx_array_get_last(const amx_array_t *array)
{
	amx_array_it_t *it = NULL;
	if (!array)
	{
		goto exit;
	}

	if (!amx_array_is_empty(array))
	{
		it = &array->buffer[array->last_used];
	}

exit:
	return it;
}

amx_array_it_t *amx_array_get_last_free(const amx_array_t *array)
{
	amx_array_it_t *it = NULL;
	if (!array)
	{
		goto exit;
	}

	size_t index = array->items;
	while(index > 0 && array->buffer[index - 1].data != NULL)
	{
		index--;
	}

	if (index > 0)
	{
		it = &array->buffer[index - 1];
	}

exit:
	return it;
}

void *amx_array_take_first_data(amx_array_t *array)
{
	void *data = NULL;
	amx_array_it_t *it = amx_array_get_first(array);
	if (it)
	{
		data = it->data;
		amx_array_set_data_at(array, amx_array_it_index(it), NULL);
	}
	return data;
}

void *amx_array_take_last_data(amx_array_t *array)
{
	void *data = NULL;
	amx_array_it_t *it = amx_array_get_last(array);
	if (it)
	{
		data = it->data;
		amx_array_set_data_at(array, amx_array_it_index(it), NULL);
	}
	return data;
}

