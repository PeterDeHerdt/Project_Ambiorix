#if !defined(__AMX_LSTACK_TYPES_H__)
#define __AMX_LSTACK_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist.h>

typedef amx_llist_t amx_lstack_t ;
typedef amx_llist_it_t amx_lstack_it_t;
typedef amx_llist_it_delete_t amx_lstack_it_delete_t;

#ifdef __cplusplus
}
#endif

#endif // AMX_LSTACK_TYPES_H
