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
 Ambiorix hash table iterator API implementation
*/

int amx_htable_it_init(amx_htable_it_t *it)
{
	int retval = -1;
	if (!it)
	{
		goto exit;
	}

	it->ait = NULL;
	it->key = NULL;
	it->next = NULL;

	retval = 0;

exit:
	return retval;
}

void amx_htable_it_clean(amx_htable_it_t *it)
{
	if (!it)
	{
		goto exit;
	}

	// remove from htable if it is in one
	amx_htable_it_take(it);
	free(it->key);
	it->key = NULL;

exit:
	return;
}

amx_htable_it_t *amx_htable_it_get_next(const amx_htable_it_t *reference)
{
	amx_htable_it_t *it = NULL;
	if (!reference || !reference->ait)
	{
		goto exit;
	}

	if (reference->next)
	{
		it = reference->next;
	}
	else
	{
		amx_array_it_t *ait = amx_array_it_get_next(reference->ait);
		if (!ait)
		{
			goto exit;
		}
		it = ait->data;
	}

exit:
	return it;
}

amx_htable_it_t *amx_htable_it_get_next_key(const amx_htable_it_t *reference)
{
	amx_htable_it_t *it = NULL;
	if (!reference || !reference->ait)
	{
		goto exit;
	}

	it = reference->next;
	while(it && strcmp(it->key, reference->key) != 0)
	{
		it = it->next;
	}

exit:
	return it;
}

void amx_htable_it_take(amx_htable_it_t *it)
{
	if (!it || !it->ait)
	{
		goto exit;
	}

	amx_htable_t *htable = (amx_htable_t *)it->ait->array;
	if (it->ait->data != it)
	{
		amx_htable_it_t *prev = it->ait->data;
		while(prev->next != it)
		{
			prev = prev->next;
		}
		prev->next = it->next;
	}
	else
	{
		if (it->next)
		{
			amx_array_it_set_data(it->ait, it->next);
			it->next = NULL;
		}
		else
		{
			amx_array_it_take_data(it->ait);
		}
	}
	it->ait = NULL;
	it->next = NULL;
	htable->items--;

exit:
	return;
}

const char *amx_htable_it_get_key(const amx_htable_it_t *it)
{
	return it?it->key:NULL;
}
