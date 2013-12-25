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
#include <string.h>

#include "amx_variant_priv.h"

/**
 @file
 @brief
 Ambiorix variant API implementation
*/

int amx_var_new(amx_var_t **var)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	*var = calloc(1, sizeof(amx_var_t));
	if (!(*var))
	{
		goto exit;
	}

	retval = amx_var_init(*var);

exit:
	return retval;
}

void amx_var_delete(amx_var_t **var)
{
	if (!var)
	{
		goto exit;
	}

	amx_var_clean(*var);
	free(*var);
	*var = NULL;

exit:
	return;
}

int amx_var_init(amx_var_t *var)
{
	int retval = -1;
	if (!var)
	{
		goto exit;
	}

	var->type_id = 0;
	var->data.data = NULL;
	amx_llist_it_init(&var->lit);
	retval = amx_htable_it_init(&var->hit);

exit:
	return retval;
}

void amx_var_clean(amx_var_t *var)
{
	if (!var)
	{
		goto exit;
	}

	amx_array_it_t *it = amx_array_get_at(amx_variant_get_types_array(), var->type_id);
	if (!it)
	{
		goto clean;
	}

	amx_var_type_t *var_type = it->data;
	if (var_type && var_type->del && var->data.data)
	{
		var_type->del(var);
	}

clean:
	var->type_id = 0;
	var->data.data = NULL;
	amx_htable_it_clean(&var->hit, NULL);
	amx_llist_it_clean(&var->lit, NULL);

exit:
	return;
}

int amx_var_copy(amx_var_t *dest, const amx_var_t *src)
{
	int retval = -1;
	if (!dest || !src)
	{
		goto exit;
	}

	// first clean the destination variant
	amx_var_clean(dest);

	// get the type function pointers of the src
	amx_var_type_t *type = amx_var_get_type(src->type_id);
	if (!type || !type->copy)
	{
		// no type found or no copy function available
		goto exit;
	}

	// call the copy function
	retval = type->copy(dest, src);

exit:
	return retval;
}

int amx_var_convert(amx_var_t *dst, const amx_var_t *src, int type_id)
{
	int retval = -1;
	if (!dst || !src)
	{
		goto exit;
	}

	// first clean the destination variant
	amx_var_clean(dst);
	// set the type of the destionation
	dst->type_id = type_id;

	amx_var_type_t *src_type = amx_var_get_type(src->type_id);
	amx_var_type_t *dst_type = amx_var_get_type(dst->type_id);
	// at least one conversion function must be available
	if (!(src_type && src_type->convert) && !(dst_type && dst_type->convert))
	{
		goto exit;
	}

	if (dst_type && dst_type->type_id < AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		// destination type is a fixed type, use source converstion function if available
		if (src_type && src_type->convert)
		{
			retval = src_type->convert(dst, src);
		}
		else 
		{
			retval = dst_type->convert(dst, src);
		}
	}
	else 
	{
		// destination is a custom type
		if (src_type && src_type->type_id >= AMX_VAR_TYPE_ID_CUSTOM_BASE)
		{
			// source is a custom type, use source conversion function if available
			if (src_type && src_type->convert)
			{
				retval = src_type->convert(dst, src);
			}
			else 
			{
				retval = dst_type->convert(dst, src);
			}
		}
		else
		{
			// source is a fixed type, use destination conversion function if available
			if (dst_type && dst_type->convert)
			{
				retval = dst_type->convert(dst, src);
			}
			else
			{
				retval = src_type->convert(dst, src);
			}
		}
	}

exit:
	return retval;
}

int amx_var_compare(const amx_var_t *var1, const amx_var_t *var2)
{
	int retval = -1;

	return retval;
}

int amx_var_get_type_id(const amx_var_t *var)
{
	return var?var->type_id:-1;
}

const char *amx_var_get_type_name(const amx_var_t *var)
{
	return var?amx_var_get_type_name_from_id(var->type_id):NULL;
}

void *amx_var_get_data_da(const amx_var_t *var)
{
	void *data = NULL;
	if (!var || var->type_id < AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		goto exit;
	}

	data = var->data.data;

exit:
	return data;
}

int amx_var_set_data_copy(amx_var_t *var, void *data, int type_id)
{
	int retval = -1;
	if (!var || type_id < AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		goto exit;
	}

	// first clean the destination variant
	amx_var_clean(var);

	// get the type function pointers of the src
	amx_var_type_t *type = amx_var_get_type(type_id);
	if (!type || !type->copy)
	{
		// no type found or no copy function available
		goto exit;
	}

	if (!data)
	{
		var->type_id = type_id;
		retval = 0;
		goto exit;
	}

	// create temp variant to hold the data
	amx_var_t source;
	amx_var_init(&source);
	source.type_id = type_id;
	source.data.data = data;
	retval = type->copy(var, &source);
	// do not clean the temp variant

exit:
	return retval;
}

int amx_var_set_data_move(amx_var_t *var, void *data, int type_id)
{
	int retval = -1;
	if (!var || type_id < AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		goto exit;
	}

	amx_var_type_t *type = amx_var_get_type(type_id);
	if (!type)
	{
		goto exit;
	}

	// clean the variant
	amx_var_clean(var);

	var->data.data = data;
	var->type_id = type_id;

	retval = 0;

exit:
	return retval;
}
