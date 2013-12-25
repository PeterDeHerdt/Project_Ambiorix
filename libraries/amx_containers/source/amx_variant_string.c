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
#define __STDC_LIMIT_MACROS
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>

#include "amx_variant_priv.h"

static int amx_var_string_convert_to_signed_int(const char *string, long long *converted)
{
	int retval = -1;
	*converted = 0; // default value

	if (!string || !(*string))
	{
		retval = 0;
		goto exit;
	}

	const char *buffer = string;
	char *endptr = NULL;
	long long result = 0;

	// skip whitespaces
	while (isspace(*buffer)) 
	{
		buffer++;
	}

	if (!isdigit(*buffer) && *buffer != '+' && *buffer != '-') 
	{
		goto exit;
	}

	errno = 0;
	result = strtoll(buffer, &endptr, 0);

	if ((errno == ERANGE && (result == LLONG_MIN || result == LLONG_MAX)) || (errno != 0 && result == 0))
	{
		goto exit;
	}
	if (endptr == buffer || *endptr != '\0')
	{
		goto exit;
	}

	*converted = result;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_unsigned_int(const char *string, unsigned long long *converted)
{
	int retval = -1;
	*converted = 0; // default value

	if (!string || !(*string))
	{
		retval = 0;
		goto exit;
	}

	const char *buffer = string;
	char *endptr = NULL;
	unsigned long long result = 0;

	// skip whitespaces
	while (isspace(*buffer)) 
	{
		buffer++;
	}

	if (!isdigit(*buffer) && *buffer != '+') 
	{
		goto exit;
	}

	errno = 0;
	result = strtoull(buffer, &endptr, 0);

	if ((errno == ERANGE && (result == 0 || result == ULLONG_MAX)) || (errno != 0 && result == 0))
	{
		goto exit;
	}
	if (endptr == buffer || *endptr != '\0')
	{
		goto exit;
	}
	*converted = result;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_int8(const char *string, int8_t *value)
{
	int retval = -1;
	long long converted = 0;
	if (amx_var_string_convert_to_signed_int(string, &converted) == -1)
	{
		goto exit;
	}

	/* verify overflow or underflow */
	if (converted > (long long)INT8_MAX || converted < (long long)INT8_MIN)
	{
		goto exit;
	}

	*value = (int8_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_int16(const char *string, int16_t *value)
{
	int retval = -1;
	long long converted = 0;
	if (amx_var_string_convert_to_signed_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > INT16_MAX || converted < INT16_MIN)
	{
		goto exit;
	}

	*value = (int16_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_int32(const char *string, int32_t *value)
{
	int retval = -1;
	long long converted = 0;
	if (amx_var_string_convert_to_signed_int(string, &converted) == -1)
	{
		goto exit;
	}

	/* verify overflow or underflow */
	if (converted > INT32_MAX || converted < INT32_MIN)
	{
		goto exit;
	}

	*value = (int32_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_int64(const char *string, int64_t *value)
{
	int retval = -1;
	long long converted = 0;
	if (amx_var_string_convert_to_signed_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > INT64_MAX || converted < INT64_MIN)
	{
		goto exit;
	}

	*value = (int64_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_uint8(const char *string, uint8_t *value)
{
	int retval = -1;
	unsigned long long converted = 0;
	if (amx_var_string_convert_to_unsigned_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > UINT8_MAX)
	{
		goto exit;
	}

	*value = (uint8_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_uint16(const char *string, uint16_t *value)
{
	int retval = -1;
	unsigned long long converted = 0;
	if (amx_var_string_convert_to_unsigned_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > UINT16_MAX)
	{
		goto exit;
	}

	*value = (uint16_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_uint32(const char *string, uint32_t *value)
{
	int retval = -1;
	unsigned long long converted = 0;
	if (amx_var_string_convert_to_unsigned_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > UINT32_MAX)
	{
		goto exit;
	}

	*value = (uint32_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_uint64(const char *string, uint64_t *value)
{
	int retval = -1;
	unsigned long long converted = 0;
	if (amx_var_string_convert_to_unsigned_int(string, &converted) == -1)
	{
		goto exit;
	}
	
	/* verify overflow or underflow */
	if (converted > UINT64_MAX)
	{
		goto exit;
	}

	*value = (uint64_t)converted;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_double(const char *string, double *value)
{
	int retval = -1;
	*value = 0; // default value

	if (!string || !(*string))
	{
		retval = 0;
		goto exit;
	}

	const char *buffer = string;
	char *endptr = NULL;
	double result = 0;

	while (isspace(*buffer)) 
	{
		buffer++;
	}

	if (!isdigit(*buffer) && *buffer != '-' && *buffer != '+')
	{
		goto exit;
	}

	errno = 0;
	result = strtod(buffer, &endptr);

	if ((errno == ERANGE && result == 0) ||
	    (errno != 0 && result == 0) ||
	    (endptr == buffer) ||
	    (*endptr != '\0')) {
		goto exit;
	}

	*value = result;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_float(const char *string, float *value)
{
	int retval = -1;
	*value = 0; // default value

	if (!string || !(*string))
	{
		retval = 0;
		goto exit;
	}

	const char *buffer = string;
	char *endptr = NULL;
	double result = 0;

	while (isspace(*buffer)) 
	{
		buffer++;
	}

	if (!isdigit(*buffer) && *buffer != '-' && *buffer != '+')
	{
		goto exit;
	}

	errno = 0;
	result = strtof(buffer, &endptr);

	if ((errno == ERANGE && result == 0) ||
	    (errno != 0 && result == 0) ||
	    (endptr == buffer) ||
	    (*endptr != '\0')) {
		goto exit;
	}

	*value = result;
	retval = 0;

exit:
	return retval;
}

static int amx_var_string_convert_to_bool(const char *string, bool *value)
{
	int retval = 0;
	*value = false;

	if (!string)
	{
		goto exit;
	}

	const char true_values[3][5] = { "true", "yes", "1" };
	for(int index = 0; index < 3; index++)
	{
		if (strcasecmp(string, true_values[index]) == 0)
		{
			*value = true;
			break;
		}
	}

exit:
	return retval;
}

static int amx_var_string_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_string_convert(amx_var_t *dst, const amx_var_t *src);
static int amx_var_string_compare(amx_var_t *var1, amx_var_t *var2);
static void amx_var_string_free(amx_var_t *var);

static amx_var_type_t amx_var_string = 
{
	.copy = amx_var_string_copy,
	.convert = amx_var_string_convert,
	.compare = amx_var_string_compare,
	.del = amx_var_string_free,
	.name = AMX_VAR_TYPE_NAME_STRING
};

static int amx_var_string_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_STRING;
	if (src->data.s)
	{
		dst->data.s = strdup(src->data.s);
		if (!dst->data.s)
		{
			retval = -1;
		}
	}
	return retval;
}

static int amx_var_string_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	// string type can only convert from a string type to another fixed type
	if (src->type_id != AMX_VAR_TYPE_ID_STRING ||
		dst->type_id >= AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		retval = -1;
		goto exit;
	}

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_string_copy(dst, src);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_string_convert_to_int8(src->data.s, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_string_convert_to_int16(src->data.s, &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_string_convert_to_int32(src->data.s, &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_string_convert_to_int64(src->data.s, &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		retval = amx_var_string_convert_to_uint8(src->data.s, &dst->data.ui8);
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		retval = amx_var_string_convert_to_uint16(src->data.s, &dst->data.ui16);
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		retval = amx_var_string_convert_to_uint32(src->data.s, &dst->data.ui32);
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		retval = amx_var_string_convert_to_uint64(src->data.s, &dst->data.ui64);
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		retval = amx_var_string_convert_to_float(src->data.s, &dst->data.f);
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		retval = amx_var_string_convert_to_double(src->data.s, &dst->data.d);
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		retval = amx_var_string_convert_to_bool(src->data.s, &dst->data.b);
	break;
	case AMX_VAR_TYPE_ID_LIST:
	// convert strings in the form of "value, value, value, ..." into a hash table
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
	// convert strings in the form of "key=value, key=value, ..." into a hash table
	break;
	case AMX_VAR_TYPE_ID_FD:
	// a string can not be converted to a file descriptor
	retval = -1;
	break;
	}

exit:
	return retval;
}

static int amx_var_string_compare(amx_var_t *var1, amx_var_t *var2)
{

}

static void amx_var_string_free(amx_var_t *var)
{
	free(var->data.s);
}

__attribute__((constructor)) static void amx_var_types_init() {
	// add the string type
	amx_var_add_type(&amx_var_string, AMX_VAR_TYPE_ID_STRING);
}

__attribute__((destructor)) static void amx_var_types_cleanup() {
	// remove the string type
	amx_var_remove_type(&amx_var_string);
}
