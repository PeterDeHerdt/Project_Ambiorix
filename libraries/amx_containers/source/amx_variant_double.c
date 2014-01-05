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

static int amx_var_double_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_double_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_int64_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_double = 
{
	.copy = amx_var_double_copy,
	.convert = amx_var_double_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_DOUBLE
};

static int amx_var_double_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_DOUBLE;
	dst->data.d = src->data.d;
	return retval;
}

static int amx_var_double_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_double_convert_to_string(src->data.d, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_double_convert_to_int8(src->data.d, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_double_convert_to_int16(src->data.d, &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_double_convert_to_int32(src->data.d, &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_double_convert_to_int64(src->data.d, &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_double_convert_to_uint8(src->data.d, &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_double_convert_to_uint16(src->data.d, &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_double_convert_to_uint32(src->data.d, &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		retval = amx_var_double_convert_to_uint64(src->data.d, &dst->data.ui64);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)src->data.d;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = src->data.d;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.d?true:false;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
		retval = amx_var_double_convert_to_fd(src->data.d, &dst->data.fd);
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

int amx_var_double_convert_to_string(double d, char **value)
{
	int retval = -1;
	*value = calloc(1,64);

	if (!(*value))
	{
		goto exit;
	}

	if (snprintf(*value, 64, "%f", d) < 0)
	{
		goto exit;
	}

	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_int8(double d, int8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (d > INT8_MAX || d < INT8_MIN)
	{
		goto exit;
	}

	*value = (int8_t)d;
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_int16(double d, int16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (d > INT16_MAX || d < INT16_MIN)
	{
		goto exit;
	}

	*value = (int16_t)d;
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_int32(double d, int32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (d > INT32_MAX || d < INT32_MIN)
	{
		goto exit;
	}

	*value = (int32_t)d;
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_int64(double d, int64_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (d > INT64_MAX || d < INT64_MIN)
	{
		goto exit;
	}

	*value = (int32_t)d;
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_uint8(double d, uint8_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabs(d) > UINT8_MAX)
	{
		goto exit;
	}

	*value = (uint8_t)fabs(d);
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_uint16(double d, uint16_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabs(d) > UINT16_MAX)
	{
		goto exit;
	}

	*value = (uint16_t)fabs(d);
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_uint32(double d, uint32_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabs(d) > UINT32_MAX)
	{
		goto exit;
	}

	*value = (uint32_t)fabs(d);
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_uint64(double d, uint64_t *value)
{
	int retval = -1;
	*value = 0;

	/* verify overflow or underflow */
	if (fabs(d) > UINT64_MAX)
	{
		goto exit;
	}

	*value = (uint64_t)fabs(d);
	retval = 0;

exit:
	return retval;
}

int amx_var_double_convert_to_fd(double d, int *value)
{
	int retval = -1;
	*value = -1;

	/* verify overflow or underflow */
	if (fabs(d) > INT_MAX)
	{
		goto exit;
	}

	int fd = (int)fabs(d);
	printf("double = %f, fd = %d\n", d, fd);
	if (fcntl(fd, F_GETFD) == -1) 
	{
		goto exit;
	}
	
	*value = (int)fabs(d);
	retval = 0;

exit:
	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the double type
	amx_var_add_type(&amx_var_double, AMX_VAR_TYPE_ID_DOUBLE);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_double);
}

int amx_var_set_double(amx_var_t *var, double number)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_DOUBLE;
	var->data.d = number;
	retval = 0;

exit:
	return retval;
}

double amx_var_get_double(const amx_var_t *var)
{
	double number = 0;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_DOUBLE);
	number = variant.data.d;

exit:
	return number;
}
