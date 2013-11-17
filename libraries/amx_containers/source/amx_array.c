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

static void amx_array_initialize(amx_array_t *array, unsigned int start_pos)
{
	amx_array_it_t *it = NULL;
	size_t size = sizeof(amx_array_it_t) + array->item_size;

	for (unsigned int index = start_pos; index < array->items; index++)
	{
		it = (amx_array_it_t *)(array->buffer + (size * index));
		it->array = array;
	}
}

int amx_array_new(amx_array_t **array, const size_t items, const size_t item_size)
{
	int retval = -1;
	if (!array || !item_size)
	{
		goto exit;
	}

	/* allocate the array structure */
	*array = calloc(1, sizeof(amx_array_t));
	if (!(*array))
	{
		goto exit;
	}

	/* set the item size and number of items in the array */
	(*array)->items = items;
	(*array)->item_size = item_size;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	/* calculate the size of the array buffer 
	   and allocate the buffer
	*/
	size_t size = sizeof(amx_array_it_t) + item_size;
	(*array)->buffer = calloc(items, size);
	if (!(*array)->buffer)
	{
		free(*array);
		*array = NULL;
		goto exit;
	}
	amx_array_initialize(*array, 0);

	retval = 0;

exit:
	return retval;
}

void amx_array_delete(amx_array_t **array)
{
	if (!array)
	{
		goto exit;
	}

	free((*array)->buffer);
	free(*array);
	*array = NULL;

exit:
	return;
}

int amx_array_init(amx_array_t *array, const size_t items, const size_t item_size)
{
	int retval = -1;
	if (!array || item_size == 0)
	{
		goto exit;
	}

	array->buffer = NULL;
	array->items = items;
	array->item_size = item_size;

	/* if no items need to be pre-allocated, leave */
	if (!items)
	{
		retval = 0;
		goto exit;
	}

	size_t size = sizeof(amx_array_it_t) + item_size;
	array->buffer = calloc(items, size);
	if (!array->buffer)
	{
		goto exit;
	}
	amx_array_initialize(array, 0);

	retval = 0;

exit:
	return retval;
}

void amx_array_clean(amx_array_t *array)
{
	if (!array)
	{
		goto exit;
	}

	free(array->buffer);
	array->items = 0;
	array->item_size = 0;

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

	size_t size = sizeof(amx_array_it_t) + array->item_size;
	size_t old_size = array->items;
	items += old_size;
	char *buffer = realloc(array->buffer, size * items);
	if (!buffer)
	{
		goto exit;
	}

	array->buffer = buffer;
	array->items = items;
	amx_array_initialize(array, old_size);

	retval = 0;

exit:
	return retval;
}

int amx_array_shrink(amx_array_t *array, size_t items)
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
		/* free the buffer, no items needed anymore */
		free(array->buffer);
		array->buffer = 0;
		array->items = 0;
		retval = 0;
		goto exit;
	}

	size_t size = sizeof(amx_array_it_t) + array->item_size;
	size_t old_size = array->items;
	items = old_size - items;
	char *buffer = realloc(array->buffer, size * items);
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

const amx_array_it_t *amx_array_get_at(const amx_array_t *array, unsigned int index)
{
	const amx_array_it_t *it = NULL;
	if (!array || !array->buffer)
	{
		goto exit;
	}

	size_t size = sizeof(amx_array_it_t) + array->item_size;
	if (index < array->items)
	{
		it = (const amx_array_it_t *)(array->buffer + (size * index));
	}

exit:
	return it;
}
