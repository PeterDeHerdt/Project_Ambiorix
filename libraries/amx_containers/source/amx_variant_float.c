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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include "amx_variant_priv.h"

static int amx_var_float_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_float_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_int64_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_float = 
{
	.copy = amx_var_float_copy,
	.convert = amx_var_float_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_FLOAT
};

static int amx_var_float_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_FLOAT;
	dst->data.f = src->data.f;
	return retval;
}

static int amx_var_float_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_float_convert_to_string(src->data.f, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_float_convert_to_int8(src->data.f, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_float_convert_to_int16(src->data.f, &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_float_convert_to_int32(src->data.f, &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_float_convert_to_int64(src->data.f, &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_float_convert_to_uint8(src->data.f, &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_float_convert_to_uint16(src->data.f, &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_float_convert_to_uint32(src->data.f, &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		retval = amx_var_float_convert_to_uint64(src->data.f, &dst->data.ui64);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = src->data.f;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)src->data.f;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.f?true:false;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
		retval = amx_var_float_convert_to_fd(src->data.f, &dst->data.fd);
	break;
	default:
		retval = -1;
	break;
	}

	return retval;
}

/*
static int amx_var_int64_compare(amx_var_t *var1, amx_var_t *var2)
{

}
*/

int amx_var_float_convert_to_string(float f, char **value)
{
	int retval = -1;
	*value = calloc(1,64);

	if (!(*value))
	{
		goto exit;
	}

	if (snprintf(*value, 64, "%f", f) < 0)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_int8(float f, int8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (f > INT8_MAX || f < INT8_MIN)
	{
		goto exit;
	}

	*value = (int8_t)f;
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_int16(float f, int16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (f > INT16_MAX || f < INT16_MIN)
	{
		goto exit;
	}

	*value = (int16_t)f;
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_int32(float f, int32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (f > INT32_MAX || f < INT32_MIN)
	{
		goto exit;
	}

	*value = (int32_t)f;
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_int64(float f, int64_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (f > INT64_MAX || f < INT64_MIN)
	{
		goto exit;
	}

	*value = (int32_t)f;
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_uint8(float f, uint8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabsf(f) > UINT8_MAX)
	{
		goto exit;
	}

	*value = (uint8_t)fabsf(f);
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_uint16(float f, uint16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabsf(f) > UINT16_MAX)
	{
		goto exit;
	}

	*value = (uint16_t)fabsf(f);
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_uint32(float f, uint32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabsf(f) > UINT32_MAX)
	{
		goto exit;
	}

	*value = (uint32_t)fabsf(f);
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_uint64(float f, uint64_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabsf(f) > UINT64_MAX)
	{
		goto exit;
	}

	*value = (uint64_t)fabsf(f);
	retval = 0;

exit:
	return retval;
}

int amx_var_float_convert_to_fd(float f, int *value)
{
	int retval = -1;
	*value = -1;

	/* verify overflow or underflow */
	if (fabsf(f) > INT_MAX)
	{
		goto exit;
	}

	if (fcntl((int)fabsf(f), F_GETFD) == -1) 
	{
		goto exit;
	}
	
	*value = (int)fabsf(f);
	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the float type
	amx_var_add_type(&amx_var_float, AMX_VAR_TYPE_ID_FLOAT);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_float);
}

int amx_var_set_float(amx_var_t *var, float number)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_FLOAT;
	var->data.f = number;
	retval = 0;

exit:
	return retval;
}

float amx_var_get_float(const amx_var_t *var)
{
	float number = 0;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_FLOAT);
	number = variant.data.f;

exit:
	return number;
}

