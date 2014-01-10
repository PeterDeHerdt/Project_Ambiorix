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

void amx_llist_var_delete(amx_llist_it_t *it)
{
	amx_var_t *var = amx_llist_it_get_data(it, amx_var_t, lit);
	amx_var_delete(&var);
}

void amx_htable_var_delete(const char *key, amx_htable_it_t *it)
{
	(void)key;
	amx_var_t *var = amx_htable_it_get_data(it, amx_var_t, hit);
	amx_var_delete(&var);
}

int amx_var_convert_to_list(const amx_var_t *src, amx_llist_t **value)
{
	int retval = -1;
	if (amx_llist_new(value) == -1)
	{
		goto exit;
	}

	amx_var_t *var = NULL;
	if (amx_var_new(&var) == -1)
	{
		amx_llist_delete(value, amx_llist_var_delete);
		goto exit;
	}

	amx_var_copy(var, src);
	amx_llist_append(*value, &var->lit);

	retval = 0;

exit:
	return retval;
}

int amx_var_convert_to_htable(const amx_var_t *src, amx_htable_t **value, const char *key)
{
	int retval = -1;
	if (amx_htable_new(value, 8) == -1)
	{
		goto exit;
	}

	amx_var_t *var = NULL;
	if (amx_var_new(&var) == -1)
	{
		amx_htable_delete(value, amx_htable_var_delete);
		goto exit;
	}

	amx_var_copy(var, src);
	amx_htable_insert(*value, key, &var->hit);

	retval = 0;

exit:
	return retval;
}

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

amx_var_t *amx_var_from_llist_it(const amx_llist_it_t *it)
{
	return it?amx_llist_it_get_data(it, amx_var_t, lit):NULL;
}

amx_var_t *amx_var_from_htable_it(const amx_htable_it_t *it)
{
	return it?amx_htable_it_get_data(it, amx_var_t, hit):NULL;
}

amx_llist_it_t *amx_var_get_llist_it(amx_var_t *var)
{
	return var?&var->lit:NULL;
}

amx_htable_it_t *amx_var_get_htable_it(amx_var_t *var)
{
	return var?&var->hit:NULL;
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
		// destination type is a fixed type, use source conversion function if available
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

/*
int amx_var_compare(const amx_var_t *var1, const amx_var_t *var2)
{
	int retval = -1;

	return retval;
}
*/

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
