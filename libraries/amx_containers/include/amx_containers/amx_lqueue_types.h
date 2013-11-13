#if !defined(__AMX_LQUEUE_TYPES_H__)
#define __AMX_LQUEUE_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist.h>

/**
 @file
 @brief
 Ambiorix linked list types header file
*/

/**
 @ingroup amx_lqueue
 @brief
 The linked queue structure.
 */
typedef amx_llist_t amx_lqueue_t;

/**
 @ingroup amx_lqueue
 @brief
 The linked queue iterator structure.
 */
typedef amx_llist_it_t amx_lqueue_it_t;

/**
 @ingroup amx_lqueue
 @brief
 Definition of the item delete function.

 A pointer to a delete function is used in the following functions @ref amx_delete_lqueue, @ref amx_clean_lqueue
 */
typedef amx_llist_it_delete_t amx_lqueue_it_delete_t;

#ifdef __cplusplus
}
#endif

#endif // AMX_LQUEUE_TYPES_H
