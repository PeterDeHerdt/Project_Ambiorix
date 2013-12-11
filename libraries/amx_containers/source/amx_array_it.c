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

#include <amx_containers/amx_array.h>

/**
 @file
 @brief
 Ambiorix array iterator API implementation
*/

amx_array_it_t *amx_array_it_get_next(const amx_array_it_t *reference)
{
	amx_array_it_t *it = NULL;
	if (!reference)
	{
		goto exit;
	}

	amx_array_t *array = reference->array;
	size_t pos = (reference - array->buffer);
	pos++;
	while(pos < array->items && !array->buffer[pos].data)
	{
		pos++;
	}

	if (pos < array->items)
	{
		it = &array->buffer[pos];
	}

exit:
	return it;
}

amx_array_it_t *amx_array_it_get_previous(const amx_array_it_t *reference)
{
	amx_array_it_t *it = NULL;
	if (!reference)
	{
		goto exit;
	}

	amx_array_t *array = reference->array;
	size_t pos = (reference - array->buffer);
	pos--;
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

unsigned int amx_array_it_index(const amx_array_it_t *it)
{
	size_t index = 0;
	if (!it)
	{
		goto exit;
	}

	index = it - it->array->buffer;

exit:
	return index;
}

int amx_array_it_set_data(amx_array_it_t *it, void *data)
{
	int retval = -1;
	if (!it || !data)
	{
		goto exit;
	}

	// set data pointer
	it->data = data;

	// update counters
	unsigned int index = amx_array_it_index(it);
	amx_array_t *array = it->array;
	if (index < array->first_used || (array->first_used == 0 && array->buffer[0].data == NULL))
	{
		array->first_used = index;
	}
	if (index > array->last_used)
	{
		array->last_used = index;
	}

	retval = 0;

exit:
	return retval;
}

void *amx_array_it_take_data(amx_array_it_t *it)
{
	void *data = NULL;
	if (!it)
	{
		goto exit;
	}
	
	if (!it->data)
	{
		goto exit;
	}

	data = it->data;
	it->data = NULL;
	unsigned int index = amx_array_it_index(it);
	amx_array_t *array = it->array;

	if (index == array->first_used)
	{
		it = amx_array_it_get_next(it);
		if (!it)
		{
			array->first_used = 0;
		}
		else
		{
			array->first_used = amx_array_it_index(it);
		}
	}
	if (index == array->last_used)
	{
		it = amx_array_it_get_previous(it);
		if (!it)
		{
			array->last_used = 0;
		}
		else
		{
			array->last_used = amx_array_it_index(it);
		}
	}

exit:
	return data;
}
