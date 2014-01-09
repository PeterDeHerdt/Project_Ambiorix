#if !defined(__AMX_CONTAINERS_CHECK_H__)
#define __AMX_CONTAINERS_CHECK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <check.h>

Suite *amx_llist_suite(void);
Suite *amx_llist_it_suite(void);
Suite *amx_lstack_suite(void);
Suite *amx_lqueue_suite(void);
Suite *amx_array_suite(void);
Suite *amx_aqueue_suite(void);
Suite *amx_astack_suite(void);
Suite *amx_rbuffer_suite(void);
Suite *amx_hash_suite(void);
Suite *amx_htable_suite(void);
Suite *amx_var_type_suite(void);
Suite *amx_var_suite(void);
Suite *amx_var_string_suite(void);
Suite *amx_var_int8_suite(void);
Suite *amx_var_int16_suite(void);
Suite *amx_var_int32_suite(void);
Suite *amx_var_int64_suite(void);
Suite *amx_var_uint8_suite(void);
Suite *amx_var_uint16_suite(void);
Suite *amx_var_uint32_suite(void);
Suite *amx_var_uint64_suite(void);
Suite *amx_var_float_suite(void);
Suite *amx_var_double_suite(void);
Suite *amx_var_bool_suite(void);
Suite *amx_var_fd_suite(void);

#ifdef __cplusplus
}
#endif

#endif // __AMX_CONTAINERS_CHECK_H__
