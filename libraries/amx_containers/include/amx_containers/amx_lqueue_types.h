#if !defined(__AMX_LQUEUE_TYPES_H__)
#define __AMX_LQUEUE_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist.h>

typedef amx_llist_t amx_lqueue_t ;
typedef amx_llist_it_t amx_lqueue_it_t;
typedef amx_llist_it_delete_t amx_lqueue_it_delete_t;

#ifdef __cplusplus
}
#endif

#endif // AMX_LQUEUE_TYPES_H
