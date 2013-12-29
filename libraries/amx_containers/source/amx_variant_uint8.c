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
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "amx_variant_priv.h"

static int amx_var_uint8_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_uint8_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_uint64_compare(amx_var_t *var1, amx_var_t *var2);

static amx_var_type_t amx_var_uint8 = 
{
	.copy = amx_var_uint8_copy,
	.convert = amx_var_uint8_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_UINT8
};

static int amx_var_uint8_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_UINT8;
	dst->data.ui8 = src->data.ui8;
	return retval;
}

static int amx_var_uint8_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		retval = amx_var_uint64_convert_to_string(src->data.ui8, &dst->data.s);
	break;
	case AMX_VAR_TYPE_ID_INT8:
		retval = amx_var_uint64_convert_to_int8(src->data.ui8, &dst->data.i8);
	break;
	case AMX_VAR_TYPE_ID_INT16:
		retval = amx_var_uint64_convert_to_int16(src->data.ui8, &dst->data.i16);
	break;
	case AMX_VAR_TYPE_ID_INT32:
		retval = amx_var_uint64_convert_to_int32(src->data.ui8, &dst->data.i32);
	break;
	case AMX_VAR_TYPE_ID_INT64:
		retval = amx_var_uint64_convert_to_int64(src->data.ui8, &dst->data.i64);
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		dst->data.ui8 = src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		dst->data.ui16 = src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		dst->data.ui32 = src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		dst->data.f = (float)src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		dst->data.d = (double)src->data.ui8;
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		dst->data.b = src->data.ui8?true:false;
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
		retval = amx_var_uint64_convert_to_fd(src->data.ui8, &dst->data.fd);
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

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the ui8 type
	amx_var_add_type(&amx_var_uint8, AMX_VAR_TYPE_ID_UINT8);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_uint8);
}

