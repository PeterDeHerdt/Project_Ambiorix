#if !defined(__AMX_LSTACK_TYPES_H__)
#define __AMX_LSTACK_TYPES_H__

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
 @ingroup amx_lstack
 @brief
 The linked stack structure.
 */
typedef amx_llist_t amx_lstack_t;

/**
 @ingroup amx_lstack
 @brief
 The linked stack iterator structure.
 */
typedef amx_llist_it_t amx_lstack_it_t;

/**
 @ingroup amx_lstack
 @brief
 Definition of the item delete function.

 A pointer to a delete function is used in the following functions @ref amx_delete_lstack, amx_clean_lstack
 */
typedef amx_llist_it_delete_t amx_lstack_it_delete_t;

#ifdef __cplusplus
}
#endif

#endif // AMX_LSTACK_TYPES_H
