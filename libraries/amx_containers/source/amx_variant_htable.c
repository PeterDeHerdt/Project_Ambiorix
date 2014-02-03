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

static int amx_var_htable_convert_to_string(amx_htable_t *htable, char **string);
static int amx_var_htable_convert_to_llist(amx_htable_t *htable, amx_llist_t **llist);

static void amx_var_htable_free(amx_var_t *var);
static int amx_var_htable_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_htable_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_htable_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_htable = 
{
	.copy = amx_var_htable_copy,
	.convert = amx_var_htable_convert,
	.compare = NULL,
	.del = amx_var_htable_free,
	.name = AMX_VAR_TYPE_NAME_HTABLE
};

static int amx_var_htable_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = -1;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_HTABLE;
	if (amx_htable_new(&dst->data.vm, src->data.vm?src->data.vm->table.items:0) == -1)
	{
		goto exit;
	}

	amx_var_t *var = NULL;
	amx_htable_for_each(it, src->data.vm)
	{
		if (amx_var_new(&var) == -1)
		{
			amx_var_clean(dst);
			goto exit;
		}
		amx_var_t *item = amx_var_from_htable_it(it);
		amx_var_copy(var, item);
		amx_htable_insert(dst->data.vm, it->key, &var->hit);
	}

	retval = 0;

exit:
	return retval;
}

static int amx_var_htable_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_htable_convert_to_string(src->data.vm, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_uint64_convert_to_int8((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_uint64_convert_to_int16((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_uint64_convert_to_int32((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_uint64_convert_to_int64((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_uint64_convert_to_uint8((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_uint64_convert_to_uint16((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_uint64_convert_to_uint32((unsigned long long)(src->data.vm?src->data.vm->items:0), &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = (unsigned long long)(src->data.vm?src->data.vm->items:0);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)(src->data.vm?src->data.vm->items:0);
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)(src->data.vm?src->data.vm->items:0);
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = (src->data.vm?(src->data.vm->items != 0):false);
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_htable_convert_to_llist(src->data.vm, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_htable_copy(dst, src);
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

static void amx_var_htable_free(amx_var_t *var)
{
	amx_htable_delete(&var->data.vm, amx_htable_var_delete);
}

static int amx_var_htable_convert_to_string(amx_htable_t *htable, char **string)
{
	int retval = -1;
	amx_var_t text;
	amx_var_init(&text);

	amx_rbuffer_t buf;
	if (amx_rbuffer_init(&buf, 64) == -1)
	{
		goto exit;
	}

	const char *sep = "";
	amx_htable_for_each(it, htable)
	{
		amx_var_t *item = amx_var_from_htable_it(it);
		if (amx_var_convert(&text, item, AMX_VAR_TYPE_ID_STRING) == -1)
		{
			amx_rbuffer_clean(&buf);
			goto exit;
		}
		if (*sep && amx_rbuffer_write(&buf, sep, strlen(sep)) == -1)
		{
			amx_rbuffer_clean(&buf);
			goto exit;
		}
		int length = strlen(it->key);
		if (amx_rbuffer_write(&buf, it->key, length) == -1)
		{
			amx_rbuffer_clean(&buf);
			goto exit;
		}
		if (amx_rbuffer_write(&buf, "=", 1) == -1)
		{
			amx_rbuffer_clean(&buf);
			goto exit;
		}
		const char *txt = amx_var_get_string_da(&text);
		if (txt)
		{
			length = strlen(txt);
			if (amx_rbuffer_write(&buf, txt, length) == -1)
			{
				amx_rbuffer_clean(&buf);
				goto exit;
			}
		}
		else
		{
			if (amx_rbuffer_write(&buf, "null", 4) == -1)
			{
				amx_rbuffer_clean(&buf);
				goto exit;
			}
		}
		sep = ",";
	}

	char eot = 0x00;
	amx_rbuffer_write(&buf, &eot, 1);
	*string = buf.buffer_start;
	retval = 0;

exit:
	amx_var_clean(&text);
	return retval;
}

static int amx_var_htable_convert_to_llist(amx_htable_t *htable, amx_llist_t **llist)
{
	int retval = -1;
	if (amx_llist_new(llist) == -1)
	{
		goto exit;
	}

	amx_var_t *data = NULL;
	amx_htable_for_each(it, htable)
	{
		amx_var_t *item = amx_var_from_htable_it(it);
		// allocate new variant and copy the item
		if (amx_var_new(&data) == -1)
		{
			amx_llist_delete(llist, amx_llist_var_delete);
			goto exit;
		}
		if (amx_var_copy(data, item) == -1)
		{
			amx_var_delete(&data);
			amx_llist_delete(llist, amx_llist_var_delete);
			goto exit;
		}
		// insert in the htable
		amx_llist_append(*llist, amx_var_get_llist_it(data));
	}

	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the htable type
	amx_var_add_type(&amx_var_htable, AMX_VAR_TYPE_ID_HTABLE);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the htable type
	amx_var_remove_type(&amx_var_htable);
}

int amx_var_set_htable_copy(amx_var_t *var, const amx_htable_t *htable)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_t *src = NULL;
	amx_var_t *dest = NULL;

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_HTABLE;

	// alloc list
	if (amx_htable_new(&var->data.vm, htable?htable->table.items:0) == -1)
	{
		goto exit;
	}

	// loop over list
	amx_htable_for_each(var_it, htable)
	{
		src = amx_var_from_htable_it(var_it);
		if (amx_var_new(&dest) == -1)
		{
			amx_htable_delete(&var->data.vm, amx_htable_var_delete);
			goto exit;
		}

		if (amx_var_copy(dest,src) == -1)
		{
			amx_htable_delete(&var->data.vm, amx_htable_var_delete);
			goto exit;
		}

		amx_htable_insert(var->data.vm, var_it->key, &dest->hit);
	}

	retval = 0;

exit:
	return retval;
}

int amx_var_set_htable_move(amx_var_t *var, amx_htable_t *htable)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_HTABLE;
	var->data.vm = htable;

	retval = 0;

exit:
	return retval;
}

amx_htable_t *amx_var_get_htable(const amx_var_t *var)
{
	amx_htable_t *htable = NULL;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_HTABLE);
	htable = variant.data.vm;

exit:
	return htable;
}

amx_htable_t *amx_var_get_htable_da(const amx_var_t *var)
{
	amx_htable_t *htable = NULL;
	if (!var || var->type_id != AMX_VAR_TYPE_ID_HTABLE)
	{
		goto exit;
	}

	htable = var->data.vm;

exit:
	return htable;
}

