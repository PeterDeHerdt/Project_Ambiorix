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

#include <amx_containers/amx_llist.h>

/**
 @file
 @brief
 Ambiorix linked list iterator API implementation
*/

int amx_llist_it_init(amx_llist_it_t *it)
{
	int retval = -1;
	if (!it)
	{
		goto exit;
	}

	it->next = NULL;
	it->prev = NULL;
	it->llist = NULL;

	retval = 0;

exit:
	return retval;
}

void amx_llist_it_clean(amx_llist_it_t *it, amx_llist_it_delete_t func)
{
	amx_llist_it_take(it);
	if (it && func)
	{
		func(it);
	}
}

void amx_llist_it_take(amx_llist_it_t *it)
{
	if (!it || !it->llist)
	{
		goto exit;
	}
	if (it->prev)
	{
		it->prev->next = it->next;
	}
	else
	{
		it->llist->head = it->next;
	}
	if (it->next)
	{
		it->next->prev = it->prev;
	}
	else
	{
		it->llist->tail = it->prev;
	}

	it->next = NULL;
	it->prev = NULL;
	it->llist = NULL;
exit:
	return;
}

int amx_llist_it_insert_before(amx_llist_it_t *reference, amx_llist_it_t *it)
{
	int retval = -1;
	if (!reference || !it || !reference->llist)
	{
		goto exit;
	}

	amx_llist_it_take(it);

	it->next = reference;
	it->prev = reference->prev;
	it->llist = reference->llist;
	reference->prev = it;

	if (it->prev)
	{
		it->prev->next = it;
	} else {
		it->llist->head = it;
	}

	retval = 0;
exit:
	return retval;
}

int amx_llist_it_insert_after(amx_llist_it_t *reference, amx_llist_it_t *it)
{
	int retval = -1;
	if (!reference || !it || !reference->llist)
	{
		goto exit;
	}

	amx_llist_it_take(it);

	it->next = reference->next;
	it->prev = reference;
	it->llist = reference->llist;
	reference->next = it;

	if (it->next)
	{
		it->next->prev = it;
	} else {
		it->llist->tail = it;
	}

	retval = 0;
exit:
	return retval;
}

unsigned int amx_llist_it_index_of(const amx_llist_it_t *it)
{
	size_t index = 0;
	if (!it || !it->llist)
	{
		index = AMX_LLIST_RANGE;
		goto exit;
	}

	amx_llist_it_t *pos = it;
	while(pos->prev) {
		index++;
		pos = pos->prev;
	}

exit:
	return index;
}
