#if !defined(__AMX_VARIANT_PRIV_H__)
#define __AMX_VARIANT_PRIV_H__

#if !defined(NO_DOXYGEN)
#define AMX_PRIVATE __attribute__ ((visibility("hidden")))
#else
#define AMX_PRIVATE
#endif

#include <amx_containers/amx_variant.h>

void AMX_PRIVATE amx_llist_var_delete(amx_llist_it_t *it);
void amx_htable_var_delete(const char *key, amx_htable_it_t *it);

int AMX_PRIVATE amx_var_add_type(amx_var_type_t *type, int index);
int AMX_PRIVATE amx_var_remove_type(amx_var_type_t *type);

amx_array_t AMX_PRIVATE *amx_variant_get_types_array();

amx_var_type_t AMX_PRIVATE *amx_var_get_type(unsigned int type_id);

int AMX_PRIVATE amx_var_int64_convert_to_string(long long i64, char **value);
int AMX_PRIVATE amx_var_int64_convert_to_int8(long long i64, int8_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_int16(long long i64, int16_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_int32(long long i64, int32_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint8(long long i64, uint8_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint16(long long i64, uint16_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint32(long long i64, uint32_t *value);

int AMX_PRIVATE amx_var_uint64_convert_to_string(unsigned long long i64, char **value);
int AMX_PRIVATE amx_var_uint64_convert_to_int8(unsigned long long ui64, int8_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int16(unsigned long long ui64, int16_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int32(unsigned long long ui64, int32_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int64(unsigned long long ui64, int64_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint8(unsigned long long ui64, uint8_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint16(unsigned long long ui64, uint16_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint32(unsigned long long ui64, uint32_t *value);

int AMX_PRIVATE amx_var_convert_to_list(const amx_var_t *src, amx_llist_t **value);
int AMX_PRIVATE amx_var_convert_to_htable(const amx_var_t *src, amx_htable_t **value, const char *key);

#endif // AMX_VARIANT_PRIV_H
