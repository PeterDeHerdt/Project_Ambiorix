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
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <amx_containers/amx_rbuffer.h>
#include "amx_variant_priv.h"

static int amx_var_llist_convert_to_string(amx_llist_t *list, char **string);
static int amx_var_llist_convert_to_htable(amx_llist_t *list, amx_htable_t **htable);

static void amx_var_list_free(amx_var_t *var);
static int amx_var_list_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_list_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_list_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_list = 
{
	.copy = amx_var_list_copy,
	.convert = amx_var_list_convert,
	.compare = NULL,
	.del = amx_var_list_free,
	.name = AMX_VAR_TYPE_NAME_LIST
};

static int amx_var_list_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = -1;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_LIST;
	if (amx_llist_new(&dst->data.vl) == -1)
	{
		goto exit;
	}

	amx_var_t *var = NULL;
	amx_llist_for_each(it, src->data.vl)
	{
		if (amx_var_new(&var) == -1)
		{
			amx_var_clean(dst);
			goto exit;
		}
		amx_var_t *item = amx_var_from_llist_it(it);
		amx_var_copy(var, item);
		amx_llist_append(dst->data.vl, &var->lit);
	}

	retval = 0;

exit:
	return retval;
}

static int amx_var_list_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_llist_convert_to_string(src->data.vl, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_uint64_convert_to_int8((unsigned long long)amx_llist_size(src->data.vl), &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_uint64_convert_to_int16((unsigned long long)amx_llist_size(src->data.vl), &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_uint64_convert_to_int32((unsigned long long)amx_llist_size(src->data.vl), &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_uint64_convert_to_int64((unsigned long long)amx_llist_size(src->data.vl), &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_uint64_convert_to_uint8((unsigned long long)amx_llist_size(src->data.vl), &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_uint64_convert_to_uint16((unsigned long long)amx_llist_size(src->data.vl), &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_uint64_convert_to_uint32((unsigned long long)amx_llist_size(src->data.vl), &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = (unsigned long long)amx_llist_size(src->data.vl);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)amx_llist_size(src->data.vl);
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)amx_llist_size(src->data.vl);
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = !amx_llist_is_empty(src->data.vl);
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_list_copy(dst, src);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_llist_convert_to_htable(src->data.vl, &dst->data.vm);
	break;
	case AMX_VAR_TYPE_ID_FD:
		dst->data.fd = -1;
		retval = -1;
	break;
	default:
		retval = -1;
	break;
	}

	return retval;
}

static void amx_var_list_free(amx_var_t *var)
{
	amx_llist_delete(&var->data.vl, amx_llist_var_delete);
}

static int amx_var_llist_convert_to_string(amx_llist_t *list, char **string)
{
	int retval = -1;
	amx_rbuffer_t buf;
	if (amx_rbuffer_init(&buf, 64) == -1)
	{
		goto exit;
	}

	char *text = NULL;
	const char *sep = "";
	amx_llist_for_each(it, list)
	{
		amx_var_t *item = amx_var_from_llist_it(it);
		text = amx_var_get_string(item);
		if (text)
		{
			if (amx_rbuffer_write(&buf, sep, strlen(sep)) == -1)
			{
				amx_rbuffer_clean(&buf);
				goto exit;
			}
			if (amx_rbuffer_write(&buf, text, strlen(text)) == -1)
			{
				amx_rbuffer_clean(&buf);
				goto exit;
			}
		}
		free(text);
		sep = ",";
	}

	*string = buf.buffer_start;
	retval = 0;

exit:
	return retval;
}

static int amx_var_llist_convert_to_htable(amx_llist_t *list, amx_htable_t **htable)
{
	int retval = -1;
	if (amx_htable_new(htable, amx_llist_size(list)) == -1)
	{
		goto exit;
	}

	amx_var_t key;
	amx_var_t *data = NULL;
	unsigned int index = 0;
	amx_var_init(&key);
	amx_llist_for_each(it, list)
	{
		amx_var_t *item = amx_var_from_llist_it(it);
		// allocate new variant and copy the item
		if (amx_var_new(&data) == -1)
		{
			amx_htable_delete(htable, amx_htable_var_delete);
			goto exit;
		}
		if (amx_var_copy(data, item) == -1)
		{
			amx_var_delete(&data);
			amx_htable_delete(htable, amx_htable_var_delete);
			goto exit;
		}
		// create a key or the htable
		amx_var_set_uint32(&key, index);
		// insert in the htable
		char *text = amx_var_get_string(&key);
		if (amx_htable_insert(*htable, text, amx_var_get_htable_it(data)) == -1)
		{
			free(text);
			amx_var_delete(&data);
			amx_htable_delete(htable, amx_htable_var_delete);
			goto exit;
		}
		free(text);
	}

	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the list type
	amx_var_add_type(&amx_var_list, AMX_VAR_TYPE_ID_LIST);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the list type
	amx_var_remove_type(&amx_var_list);
}
