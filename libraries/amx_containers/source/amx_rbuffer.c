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
#include <unistd.h>

#include <amx_containers/amx_rbuffer.h>

/**
 @file
 @brief
 Ambiorix ring buffer API implementation
*/

int amx_rbuffer_new(amx_rbuffer_t **rb, size_t size)
{
	int retval = -1;
	if (!rb)
	{
		goto exit;
	}

	*rb = calloc(1, sizeof(amx_rbuffer_t));
	if (!(*rb))
	{
		goto exit;
	}

	retval = amx_rbuffer_init(*rb, size);

exit:
	return retval;
}

void amx_rbuffer_delete(amx_rbuffer_t **rb)
{
	if (!rb)
	{
		goto exit;
	}

	amx_rbuffer_clean(*rb);
	free(*rb);
	*rb = NULL;

exit:
	return;
}

int amx_rbuffer_init(amx_rbuffer_t *rb, size_t size)
{
	int retval = -1;
	if (!rb)
	{
		goto exit;
	}

	rb->buffer_start = calloc(1, size);
	if (!rb->buffer_start)
	{
		free(rb);
		rb = NULL;
		goto exit;
	}

	rb->buffer_end = rb->buffer_start + size;
	rb->read_pos = rb->buffer_start;
	rb->write_pos = rb->buffer_start;

	retval = 0;

exit:
	return retval;
}

void amx_rbuffer_clean(amx_rbuffer_t *rb)
{
	if (!rb)
	{
		goto exit;
	}

	free(rb->buffer_start);
	rb->buffer_start = NULL;
	rb->buffer_end = NULL;
	rb->read_pos = NULL;
	rb->write_pos = NULL;

exit:
	return;
}

int amx_rbuffer_grow(amx_rbuffer_t *rb, size_t size)
{
	int retval = -1;
	if (!rb)
	{
		goto exit;
	}

	size_t read_pos = rb->read_pos - rb->buffer_start;
	size_t write_pos = rb->write_pos - rb->buffer_start;
	
	size_t new_size = (rb->buffer_end - rb->buffer_start) + size;
	char *new_buffer = realloc(rb->buffer_start, new_size);
	if (!new_buffer)
	{
		goto exit;
	}

	// if the read pointer is after the write pointer, the read pointer must be move the number of bytes
	// the buffer has grown, and the data has t be move
	if (rb->read_pos > rb->write_pos)
	{
		memmove(rb->buffer_start + read_pos, rb->buffer_start + size, size);
		read_pos += size;
	}

	rb->buffer_start = new_buffer;
	rb->buffer_end = rb->buffer_start + new_size;
	rb->read_pos = rb->buffer_start + read_pos;
	rb->write_pos = rb->buffer_start + write_pos;

	retval = 0;

exit:
	return retval;
}

int amx_rbuffer_shrink(amx_rbuffer_t *rb, size_t size)
{
	int retval = -1;
	if (!rb)
	{
		goto exit;
	}

	size_t buffer_size = rb->buffer_end - rb->buffer_start;
	if (size > buffer_size)
	{
		goto exit;
	}

	if (size == buffer_size)
	{
		amx_rbuffer_clean(rb);
		retval = 0;
		goto exit;
	}

	size_t new_size = (rb->buffer_end - rb->buffer_start) - size;
	if (rb->read_pos > rb->write_pos)
	{
		size_t bytes = rb->read_pos - rb->buffer_start;
		size_t move = (size > bytes)?bytes:size;
		memmove(rb->buffer_start + move, rb->read_pos, rb->buffer_end - rb->read_pos);
		rb->read_pos -= move;
		if (rb->read_pos > rb->buffer_start)
		{
			if (rb->write_pos > rb->read_pos)
			{
				rb->write_pos = rb->read_pos - 1;
			}
		}
		else 
		{
			rb->write_pos  = rb->buffer_start + new_size;
		}
	} else {
		size_t move = rb->write_pos - rb->read_pos;
		memmove(rb->buffer_start, rb->read_pos, move);
		rb->read_pos = rb->buffer_start;
		if (move > new_size)
		{
			rb->write_pos = rb->buffer_start + new_size;
		}
		else
		{
			rb->write_pos = rb->buffer_start + move;
		}
	}

	size_t read_pos = rb->read_pos - rb->buffer_start;
	size_t write_pos = rb->write_pos - rb->buffer_start;

	char *new_buffer = realloc(rb->buffer_start, new_size);

	rb->buffer_start = new_buffer;
	rb->buffer_end = rb->buffer_start + new_size;
	rb->read_pos = rb->buffer_start + read_pos;
	rb->write_pos = rb->buffer_start + write_pos;

	retval = 0;

exit:
	return retval;
}

ssize_t amx_rbuffer_read(amx_rbuffer_t *rb, void *buf, size_t count)
{
	ssize_t retval = -1;

	return retval;
}

ssize_t amx_rbuffer_write(amx_rbuffer_t *rb, const void *buf, size_t count)
{
	ssize_t retval = -1;

	return retval;
}

bool amx_rbuffer_is_empty(const amx_rbuffer_t *rb)
{
	bool retval = true;

	return retval;
}

size_t amx_rbuffer_capacity(const amx_rbuffer_t *rb)
{
	size_t retval = 0;

	return retval;
}

size_t amx_rbuffer_size(const amx_rbuffer_t *rb)
{
	size_t retval = 0;

	return retval;
}
