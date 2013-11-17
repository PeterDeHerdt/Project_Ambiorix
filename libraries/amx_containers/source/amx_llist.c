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
 Ambiorix linked list API implementation
*/

int amx_llist_new(amx_llist_t **llist)
{
	int retval = -1;
	if (!llist)
	{
		goto exit;
	}

	*llist = calloc(1, sizeof(amx_llist_t));
	if (!*llist)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

void amx_llist_delete(amx_llist_t **llist, amx_llist_it_delete_t func)
{
	if (!llist)
	{
		goto exit;
	}

	amx_llist_clean(*llist, func);
	free(*llist);
	*llist = NULL;
exit:
	return;
}

int amx_llist_init(amx_llist_t *llist)
{
	int retval = -1;

	if (!llist) {
		goto exit;
	}

	llist->head = NULL;
	llist->tail = NULL;

	retval = 0;

exit:
	return retval;
}

void amx_llist_clean(amx_llist_t *llist, amx_llist_it_delete_t func)
{
	amx_llist_it_t *it = amx_llist_get_first(llist);
	while(it)
	{
		amx_llist_it_take(it);
		if (func)
		{
			func(it);
		}
		it = amx_llist_get_first(llist);
	}
}

size_t amx_llist_size(const amx_llist_t *llist) {
	size_t count = 0;

	// no check on null pointer is needed here.
	// amx_llist_first will return null anyway.
	for(amx_llist_it_t *it = amx_llist_get_first(llist); it; it = amx_llist_it_get_next(it))
	{
		count++;
	}

	return count;
}

bool amx_llist_is_empty(const amx_llist_t *llist)
{
	return llist?(llist->head == NULL):true;
}

int amx_llist_append(amx_llist_t *llist, amx_llist_it_t *it)
{
	int retval = -1;
	if (!llist || !it)
	{
		goto exit;
	}

	if (it->llist)
	{
		amx_llist_it_take(it);
	}

	it->llist = llist;
	it->prev = llist->tail;
	it->next = NULL;

	if (llist->tail)
	{
		llist->tail->next = it;
		llist->tail = it;
	}
	else
	{
		llist->tail = it;
		llist->head = it;
	}

	retval = 0;

exit:
	return retval;
}

int amx_llist_prepend(amx_llist_t *llist, amx_llist_it_t *it)
{
	int retval = -1;
	if (!llist || !it)
	{
		goto exit;
	}

	if (it->llist)
	{
		amx_llist_it_take(it);
	}

	it->llist = llist;
	it->next = llist->head;
	it->prev = NULL;

	if (llist->head)
	{
		llist->head->prev = it;
		llist->head = it;
	}
	else
	{
		llist->tail = it;
		llist->head = it;
	}

	retval = 0;
exit:
	return retval;
}

amx_llist_it_t *amx_llist_get_at(amx_llist_t *llist, unsigned int index)
{
	amx_llist_it_t* it = NULL;
	size_t count = 0;

	// no check on null pointer is needed here.
	// amx_llist_first will return null anyway.
	for(it = amx_llist_get_first(llist); it; it = amx_llist_it_get_next(it))
	{
		if (count == index)
		{
			break;
		}
		count++;
	}

	return it;
}

int amx_llist_set_at(amx_llist_t *llist, size_t index, amx_llist_it_t *it)
{
	int retval = -1;
	amx_llist_it_t* reference = NULL;

	size_t count = 0;

	// no check on null pointer is needed here.
	// amx_llist_first will return null anyway.
	for(reference = amx_llist_get_first(llist); reference; reference = amx_llist_it_get_next(reference))
	{
		if (count == index)
		{
			break;
		}
		count++;
	}

	if (!reference && index == count)
	{
		retval = amx_llist_append(llist, it);
	} else {
		retval = amx_llist_it_insert_before(reference, it);
	}
	return retval;
}
