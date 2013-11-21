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

static void amx_array_clean_items(amx_array_t *array, unsigned int start_pos, amx_array_it_delete_t func)
{
	if (!func)
	{
		goto exit;
	}

	amx_array_it_t *it = NULL;
	for (unsigned int index = start_pos; index < array->items; index++)
	{
		it = &(array->buffer[index]);
		if (it->data)
		{
			func(it);
		}
		it->data = NULL;
	}
	
exit:
	return;
}

static int amx_array_realloc(amx_array_t *array, size_t items)
{
	int retval = -1;
	amx_array_it_t *buffer = realloc(array->buffer, sizeof(amx_array_it_t) * items);
	if (!buffer)
	{
		goto exit;
	}

	array->buffer = buffer;
	array->items = items;
	retval = 0;

exit:
	return retval;
}

static size_t amx_array_calculate_last_used(amx_array_t *array)
{
	size_t index =array->items - 1;
	while(index > 0 && !array->buffer[index].data)
	{
		index--;
	}

	return index;
}

int amx_array_new(amx_array_t **array, const size_t items)
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
	(*array)->last_used = 0;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	/* allocate the buffer */
	(*array)->buffer = calloc(items, sizeof(amx_array_it_t));
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

	amx_array_clean_items(*array, 0, func);

	free((*array)->buffer);
	free(*array);
	*array = NULL;

exit:
	return;
}

int amx_array_init(amx_array_t *array, const size_t items)
{
	int retval = -1;
	if (!array)
	{
		goto exit;
	}

	array->buffer = NULL;
	array->items = items;
	array->last_used = 0;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	array->buffer = calloc(items, sizeof(amx_array_it_t));
	if (!array->buffer)
	{
		goto exit;
	}

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

	amx_array_clean_items(array, 0, func);

	free(array->buffer);
	array->buffer = NULL;
	array->items = 0;

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

	retval = 0;

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

	amx_array_clean_items(array, array->items - items, func);
	retval = amx_array_realloc(array, array->items - items);
	array->last_used = amx_array_calculate_last_used(array);

exit:
	return retval;
}

amx_array_it_t *amx_array_set_data_at(amx_array_t *array, unsigned int index, void *data)
{
	amx_array_it_t *it = amx_array_get_at(array, index);
	if (!it)
	{
		goto exit;
	}

	it->data = data;
	array->last_used = (array->last_used < index)?index:array->last_used;

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

	size_t pos = 0;
	while(pos < array->items && !array->buffer[pos].data)
	{
		pos++;
	}

	if (pos < array->items && array->buffer[pos].data)
	{
		it = &array->buffer[pos];
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

	size_t pos = array->items - 1;
	while(pos > 0 && !array->buffer[pos].data)
	{
		pos--;
	}

	if (array->buffer[pos].data)
	{
		it = &array->buffer[pos];
	}

exit:
	return it;
}

