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

static int amx_var_fd_copy(amx_var_t *dst, const amx_var_t *src);
static int amx_var_fd_convert(amx_var_t *dst, const amx_var_t *src);
//static int amx_var_fd_compare(amx_var_t *var1, amx_var_t *var2);

static int amx_var_fd_convert_to_bool(int fd, bool *boolean);

static amx_var_type_t amx_var_fd = 
{
	.copy = amx_var_fd_copy,
	.convert = amx_var_fd_convert,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_FD
};

static int amx_var_fd_copy(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	// make copy
	dst->type_id = AMX_VAR_TYPE_ID_FD;
	dst->data.fd = src->data.fd;
	return retval;
}

static int amx_var_fd_convert(amx_var_t *dst, const amx_var_t *src)
{
	int retval = 0;
	int fd_flags = fcntl(src->data.fd, F_GETFD);
	printf("fd flags = %d\n", fd_flags);
	bool valid_fd = (fd_flags==-1)?false:true;

	switch(dst->type_id)
	{
	case AMX_VAR_TYPE_ID_VOID:
		dst->data.data = NULL;
	break;
	case AMX_VAR_TYPE_ID_STRING:
		if (valid_fd)
		{
			retval = amx_var_int64_convert_to_string((long long)src->data.fd, &dst->data.s);
		}
		else 
		{
			retval = amx_var_int64_convert_to_string((long long)-1, &dst->data.s);
		}
	break;
	case AMX_VAR_TYPE_ID_INT8:
		if (valid_fd)
		{
			retval = amx_var_int64_convert_to_int8((long long)src->data.fd, &dst->data.i8);
		}
		else
		{
			dst->data.i8 = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_INT16:
		if (valid_fd)
		{
			retval = amx_var_int64_convert_to_int16((long long)src->data.fd, &dst->data.i16);
		}
		else
		{
			dst->data.i16 = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_INT32:
		if (valid_fd)
		{
			retval = amx_var_int64_convert_to_int32((long long)src->data.fd, &dst->data.i32);
		}
		else
		{
			dst->data.i32 = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_INT64:
		if (valid_fd)
		{
			dst->data.i64 = src->data.fd;
		}
		else
		{
			dst->data.i64 = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_UINT8:
		dst->data.ui8 = UINT8_MAX;
		retval = -1;
	break;
	case AMX_VAR_TYPE_ID_UINT16:
		dst->data.ui16 = UINT16_MAX;
		retval = -1;
	break;
	case AMX_VAR_TYPE_ID_UINT32:
		dst->data.ui32 = UINT32_MAX;
		retval = -1;
	break;
	case AMX_VAR_TYPE_ID_UINT64:
		dst->data.ui64 = UINT64_MAX;
		retval = -1;
	break;
	case AMX_VAR_TYPE_ID_FLOAT:
		if (valid_fd)
		{
			dst->data.f = (float)src->data.fd;
		}
		else
		{
			dst->data.f = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_DOUBLE:
		if (valid_fd)
		{
			dst->data.d = (double)src->data.fd;
		}
		else
		{
			dst->data.d = -1;
		}
	break;
	case AMX_VAR_TYPE_ID_BOOL:
		retval = amx_var_fd_convert_to_bool(src->data.fd, &dst->data.b);
	break;
	case AMX_VAR_TYPE_ID_LIST:
		retval = amx_var_convert_to_list(src, &dst->data.vl);
	break;
	case AMX_VAR_TYPE_ID_HTABLE:
		retval = amx_var_convert_to_htable(src, &dst->data.vm, "1");
	break;
	case AMX_VAR_TYPE_ID_FD:
		dst->data.fd = src->data.fd;
	break;
	default:
		retval = -1;
	break;
	}

	return retval;
}

static int amx_var_fd_convert_to_bool(int fd, bool *boolean)
{
	int retval = 0;
	if (fcntl(fd, F_GETFD) == -1) 
	{
		*boolean = false;
	}
	else
	{
		*boolean = true;
	}

	return retval;
}

__attribute__((constructor)) static void amx_var_types_init()
{
	// add the fd type
	amx_var_add_type(&amx_var_fd, AMX_VAR_TYPE_ID_FD);
}

__attribute__((destructor)) static void amx_var_types_cleanup()
{
	// remove the string type
	amx_var_remove_type(&amx_var_fd);
}

int amx_var_set_fd(amx_var_t *var, int fd)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(var);
	var->type_id = AMX_VAR_TYPE_ID_FD;
	if (fcntl(fd, F_GETFD) == -1) 
	{
		var->data.fd = -1;
		goto exit;
	}
	else
	{
		var->data.fd = fd;
	}
	retval = 0;

exit:
	return retval;
}

int amx_var_get_fd(const amx_var_t *var)
{
	int fd = -1;
	if (!var)
	{
		goto exit;
	}

	amx_var_t variant;
	amx_var_init(&variant);
	if (amx_var_convert(&variant, var, AMX_VAR_TYPE_ID_FD) != -1)
	{
		fd = variant.data.fd;
	}

exit:
	return fd;
}
