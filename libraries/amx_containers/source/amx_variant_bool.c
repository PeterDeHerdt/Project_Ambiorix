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

#include "amx_variant_priv.h"

static int amx_var_bool_convert_to_string(bool boolean, char **value);

static int amx_var_bool_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_bool_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_bool_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_bool = 
{
	.copy = amx_var_bool_copy,
	.convert = amx_var_bool_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_BOOL
};

static int amx_var_bool_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_BOOL;
	dst->data.b = src->data.b;
	return retval;
}

static int amx_var_bool_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_bool_convert_to_string(src->data.b, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		dst->data.i8 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_INT16:
		dst->data.i16 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_INT32:
		dst->data.i32 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_INT64:
		dst->data.i64 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		dst->data.ui8 = src->data.b?1:0;
		break;
	case AMX_VAR_TYPE_ID_UINT16:
		dst->data.ui16 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		dst->data.ui32 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = src->data.b?1:0;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.b;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
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

static int amx_var_bool_convert_to_string(bool boolean, char **value)
{
	int retval = -1;
	*value = calloc(1,6);

	if (!(*value))
	{
		goto exit;
	}

	if (snprintf(*value, 6, "%s", boolean?"true":"false") < 0)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the bool type
	amx_var_add_type(&amx_var_bool, AMX_VAR_TYPE_ID_BOOL);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the bool type
	amx_var_remove_type(&amx_var_bool);
}

int amx_var_set_bool(amx_var_t *var, bool boolean)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_BOOL;
	var->data.b = boolean;
	retval = 0;

exit:
	return retval;
}

bool amx_var_get_bool(const amx_var_t *var)
{
	bool boolean = false;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_BOOL);
	boolean = variant.data.b;

exit:
	return boolean;
}
