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

int AMX_PRIVATE amx_var_int64_convert_to_string(long long i64, char **value);
int AMX_PRIVATE amx_var_int64_convert_to_int8(long long i64, int8_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_int16(long long i64, int16_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_int32(long long i64, int32_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint8(long long i64, uint8_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint16(long long i64, uint16_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_uint32(long long i64, uint32_t *value);
int AMX_PRIVATE amx_var_int64_convert_to_fd(long long i64, int *value);

int AMX_PRIVATE amx_var_uint64_convert_to_string(unsigned long long i64, char **value);
int AMX_PRIVATE amx_var_uint64_convert_to_int8(unsigned long long ui64, int8_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int16(unsigned long long ui64, int16_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int32(unsigned long long ui64, int32_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_int64(unsigned long long ui64, int64_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint8(unsigned long long ui64, uint8_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint16(unsigned long long ui64, uint16_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_uint32(unsigned long long ui64, uint32_t *value);
int AMX_PRIVATE amx_var_uint64_convert_to_fd(unsigned long long ui64, int *value);

int AMX_PRIVATE amx_var_float_convert_to_string(float f, char **value);
int AMX_PRIVATE amx_var_float_convert_to_int8(float f, int8_t *value);
int AMX_PRIVATE amx_var_float_convert_to_int16(float f, int16_t *value);
int AMX_PRIVATE amx_var_float_convert_to_int32(float f, int32_t *value);
int AMX_PRIVATE amx_var_float_convert_to_int64(float f, int64_t *value);
int AMX_PRIVATE amx_var_float_convert_to_uint8(float f, uint8_t *value);
int AMX_PRIVATE amx_var_float_convert_to_uint16(float f, uint16_t *value);
int AMX_PRIVATE amx_var_float_convert_to_uint32(float f, uint32_t *value);
int AMX_PRIVATE amx_var_float_convert_to_uint64(float f, uint64_t *value);
int AMX_PRIVATE amx_var_float_convert_to_fd(float f, int *value);

int AMX_PRIVATE amx_var_double_convert_to_string(double d, char **value);
int AMX_PRIVATE amx_var_double_convert_to_int8(double d, int8_t *value);
int AMX_PRIVATE amx_var_double_convert_to_int16(double d, int16_t *value);
int AMX_PRIVATE amx_var_double_convert_to_int32(double d, int32_t *value);
int AMX_PRIVATE amx_var_double_convert_to_int64(double d, int64_t *value);
int AMX_PRIVATE amx_var_double_convert_to_uint8(double d, uint8_t *value);
int AMX_PRIVATE amx_var_double_convert_to_uint16(double d, uint16_t *value);
int AMX_PRIVATE amx_var_double_convert_to_uint32(double d, uint32_t *value);
int AMX_PRIVATE amx_var_double_convert_to_uint64(double d, uint64_t *value);
int AMX_PRIVATE amx_var_double_convert_to_fd(double d, int *value);

int AMX_PRIVATE amx_var_convert_to_list(const amx_var_t *src, amx_llist_t **value);
int AMX_PRIVATE amx_var_convert_to_htable(const amx_var_t *src, amx_htable_t **value, const char *key);

#endif // AMX_VARIANT_PRIV_H
