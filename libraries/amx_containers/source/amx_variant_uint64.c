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

#define __STDC_LIMIT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "amx_variant_priv.h"

static int amx_var_uint64_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_uint64_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_uint64_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_uint64 = 
{
	.copy = amx_var_uint64_copy,
	.convert = amx_var_uint64_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_UINT64
};

static int amx_var_uint64_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_UINT64;
	dst->data.ui64 = src->data.ui64;
	return retval;
}

static int amx_var_uint64_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_uint64_convert_to_string(src->data.ui64, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_uint64_convert_to_int8(src->data.ui64, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_uint64_convert_to_int16(src->data.ui64, &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_uint64_convert_to_int32(src->data.ui64, &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_uint64_convert_to_int64(src->data.ui64, &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_uint64_convert_to_uint8(src->data.ui64, &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_uint64_convert_to_uint16(src->data.ui64, &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_uint64_convert_to_uint32(src->data.ui64, &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = src->data.ui64;
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)src->data.ui64;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)src->data.ui64;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.ui64?true:false;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
	break;
	default:
		retval = -1;
	break;
	}

	return retval;
}

/*
static int amx_var_uint64_compare(amx_var_t *var1, amx_var_t *var2)
{

}
*/

int amx_var_uint64_convert_to_string(unsigned long long ui64, char **value)
{
	int retval = -1;
	*value = calloc(1,64);

	if (!(*value))
	{
		goto exit;
	}

	if (snprintf(*value, 64, "%llu", ui64) < 0)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_int8(unsigned long long ui64, int8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > INT8_MAX)
	{
		goto exit;
	}

	*value = (int8_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_int16(unsigned long long ui64, int16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > INT16_MAX)
	{
		goto exit;
	}

	*value = (int16_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_int32(unsigned long long ui64, int32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > INT32_MAX)
	{
		goto exit;
	}

	*value = (int32_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_int64(unsigned long long ui64, int64_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > INT64_MAX)
	{
		goto exit;
	}

	*value = (int64_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_uint8(unsigned long long ui64, uint8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > UINT8_MAX)
	{
		goto exit;
	}

	*value = (uint8_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_uint16(unsigned long long ui64, uint16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > UINT16_MAX)
	{
		goto exit;
	}

	*value = (uint16_t)ui64;
	retval = 0;

exit:
	return retval;
}

int amx_var_uint64_convert_to_uint32(unsigned long long ui64, uint32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow */
	if (ui64 > UINT32_MAX)
	{
		goto exit;
	}

	*value = (uint32_t)ui64;
	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the ui64 type
	amx_var_add_type(&amx_var_uint64, AMX_VAR_TYPE_ID_UINT64);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_uint64);
}
