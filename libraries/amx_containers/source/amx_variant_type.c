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

static amx_htable_t amx_variant_types;
static amx_array_t amx_array_types;

static amx_var_type_t amx_var_void = 
{
	.copy = NULL,
	.convert = NULL,
	.compare = NULL,
	.del = NULL,
	.name = AMX_VAR_TYPE_NAME_VOID
};

static int amx_var_allocate_types()
{
	int retval = -1;
	if (amx_htable_init(&amx_variant_types, AMX_VAR_TYPE_ID_CUSTOM_BASE) == -1)
	{
		goto exit;
	}
	if (amx_array_init(&amx_array_types, AMX_VAR_TYPE_ID_CUSTOM_BASE) == -1)
	{
		goto exit;
	}
	retval = 0;

exit:
	return retval;
}

static amx_array_it_t *amx_var_get_type_position(int index)
{
	amx_array_it_t *ait = NULL;
	if (index == -1)
	{
		// no index specified - take first free item after fixed types
		ait = amx_array_get_at(&amx_array_types, AMX_VAR_TYPE_ID_CUSTOM_BASE - 1);
		ait = amx_array_it_get_next_free(ait);
		// insert in array
		if (!ait)
		{
			// no more free positions, grow array and add
			if (amx_array_grow(&amx_array_types, 5) == -1)
			{
				goto exit;
			}
			// after growing the pointers can be changed, fetch the custom base again
			ait = amx_array_get_at(&amx_array_types, AMX_VAR_TYPE_ID_CUSTOM_BASE - 1);
			ait = amx_array_it_get_next_free(ait);
		}
	}
	else
	{
		// index specified 
		// this can be only called from inside (fixed types)
		// no need to check if there is already a type registered here
		ait = amx_array_get_at(&amx_array_types, abs(index));
	}

exit:
	return ait;
}

int amx_var_add_type(amx_var_type_t *type, int index)
{
	int type_id = -1;

	// check if the type is already registered
	amx_htable_it_t *hit = amx_htable_get(&amx_variant_types, type->name);
	if (hit == &type->hit)
	{
		type_id = type->type_id;
		goto exit;
	}

	// already a type registered with that name
	if (hit)
	{
		goto exit;
	}

	if (amx_variant_types.items == 0)
	{
		// nothing allocated yet - pre-allocate all fixed items
		if (amx_var_allocate_types() == -1)
		{
			goto exit;
		}
	}

	amx_array_it_t *ait = amx_var_get_type_position(index);
	if (!ait)
	{
		goto exit;
	}

	// insert in array
	amx_array_it_set_data(ait, type);

	// insert in hash table
	amx_htable_it_init(&type->hit);
	if (amx_htable_insert(&amx_variant_types, type->name, &type->hit) == -1)
	{
		// remove from array
		amx_array_it_take_data(ait);
		goto exit;
	}

	type_id = type->type_id = amx_array_it_index(ait);

exit:
	return type_id;
}

int amx_var_remove_type(amx_var_type_t *type)
{
	int retval = -1;
	amx_htable_it_t *hit = amx_htable_get(&amx_variant_types, type->name);
	if (!hit || hit != &type->hit)
	{
		// type not found
		goto exit;
	}

	// remove from the table
	amx_htable_it_take(hit);
	amx_htable_it_clean(hit, NULL);

	// remove from array
	amx_array_it_t *it = amx_array_get_at(&amx_array_types, type->type_id);
	amx_array_it_take_data(it);

	type->type_id = -1;

	// if hash table is empty, no more types are registered, clear buffers
	if (amx_variant_types.items == 0)
	{
		// no more registered types - clean buffers
		amx_htable_clean(&amx_variant_types, NULL);
		amx_array_clean(&amx_array_types, NULL);
	}

	retval = 0;

exit:
	return retval;
}

amx_array_t *amx_variant_get_types_array()
{
	return &amx_array_types;
}

amx_var_type_t *amx_var_get_type(unsigned int type_id)
{
	amx_var_type_t *type = NULL;
	amx_array_it_t *ait = amx_array_get_at(&amx_array_types, type_id);
	if (!ait)
	{
		goto exit;
	}

	type = ait->data;

exit:
	return type;
}

__attribute__((constructor)) static void amx_var_types_init() {
	// add the void type
	amx_var_add_type(&amx_var_void, AMX_VAR_TYPE_ID_VOID);
}

__attribute__((destructor)) static void amx_var_types_cleanup() {
	// remove the void type
	amx_var_remove_type(&amx_var_void);
}

int amx_var_register_type(amx_var_type_t *type)
{
	int retval = -1;
	if (!type)
	{
		goto exit;
	}

	retval = amx_var_add_type(type, -1);

exit:
	return retval;
}

int amx_var_unregister_type(amx_var_type_t *type)
{
	int retval = -1;
	if (!type)
	{
		goto exit;
	}

	if (type->type_id < AMX_VAR_TYPE_ID_CUSTOM_BASE)
	{
		goto exit;
	}

	retval = amx_var_remove_type(type);

exit:
	return retval;
}

const char *amx_var_get_type_name_from_id(int type_id)
{
	const char *name = NULL;
	amx_array_it_t *ait = amx_array_get_at(&amx_array_types, type_id);
	if (!ait || !ait->data)
	{
		goto exit;
	}

	amx_var_type_t *type = ait->data;
	name = type->name;

exit:
	return name;
}

int amx_var_get_type_id_from_name(const char *name)
{
	int type_id = -1;
	if (!name || !(*name))
	{
		goto exit;
	}

	amx_htable_it_t *hit = amx_htable_get(&amx_variant_types, name);
	if (!hit)
	{
		goto exit;
	}

	amx_var_type_t *type = amx_htable_it_get_data(hit, amx_var_type_t, hit);
	type_id = type->type_id;

exit:
	return type_id;
}
