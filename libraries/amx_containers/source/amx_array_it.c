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

size_t amx_array_it_index(const amx_array_it_t *it)
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
