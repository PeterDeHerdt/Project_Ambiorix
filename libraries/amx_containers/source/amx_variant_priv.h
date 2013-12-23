#if !defined(__AMX_VARIANT_PRIV_H__)
#define __AMX_VARIANT_PRIV_H__

#if !defined(NO_DOXYGEN)
#define AMX_PRIVATE __attribute__ ((visibility("hidden")))
#else
#define AMX_PRIVATE
#endif

#include <amx_containers/amx_variant.h>

int AMX_PRIVATE amx_var_add_type(amx_var_type_t *type, int index);
int AMX_PRIVATE amx_var_remove_type(amx_var_type_t *type);

amx_array_t AMX_PRIVATE *amx_variant_get_types_array();

amx_var_type_t AMX_PRIVATE *amx_var_get_type(unsigned int type_id);

#endif // AMX_VARIANT_PRIV_H
