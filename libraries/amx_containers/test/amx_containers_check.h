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

#ifdef __cplusplus
}
#endif

#endif // __AMX_CONTAINERS_CHECK_H__
