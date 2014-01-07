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

static int amx_var_int16_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_int16_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_int16_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_int16 = 
{
	.copy = amx_var_int16_copy,
	.convert = amx_var_int16_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_INT16
};

static int amx_var_int16_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_INT16;
	dst->data.i16 = src->data.i16;
	return retval;
}

static int amx_var_int16_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_int64_convert_to_string((long long)src->data.i16, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_int64_convert_to_int8((long long)src->data.i16, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		dst->data.i16 = src->data.i16;
	break;
	case AMX_VAR_TYPE_ID_INT32:
		dst->data.i32 = src->data.i16;
	break;
	case AMX_VAR_TYPE_ID_INT64:
		dst->data.i64 = src->data.i16;
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_int64_convert_to_uint8((long long)src->data.i16, &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		dst->data.ui16 = abs(src->data.i16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		dst->data.ui32 = abs(src->data.i16);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = abs(src->data.i16);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)src->data.i16;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)src->data.i16;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.i16?true:false;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
		retval = amx_var_int64_convert_to_fd((long long)src->data.i16, &dst->data.fd);
	break;
	default:
		retval = -1;
	break;
	}

	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the i16 type
	amx_var_add_type(&amx_var_int16, AMX_VAR_TYPE_ID_INT16);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_int16);
}

int amx_var_set_int16(amx_var_t *var, int16_t number)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_INT16;
	var->data.i16 = number;
	retval = 0;

exit:
	return retval;
}

int16_t amx_var_get_int16(const amx_var_t *var)
{
	int16_t number = 0;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_INT16);
	number = variant.data.i16;

exit:
	return number;
}
