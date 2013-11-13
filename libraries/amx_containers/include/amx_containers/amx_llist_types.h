#if !defined(__AMX_LLIST_TYPES_H__)
#define __AMX_LLIST_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define AMX_LLIST_RANGE UINT32_MAX

/**
 @file
 @brief
 Ambiorix linked list types header file
*/

/**
 @ingroup amx_llist
 @brief
 The linked list iterator structure.
 */
typedef struct _amx_llist_it
{
	struct _amx_llist_it *next; /**< Pointer to the next item in the linked list */
	struct _amx_llist_it *prev; /**< Pointer to the previous item in the linked list */
	struct _amx_llist *llist;   /**< Pointer to the linked list */
} amx_llist_it_t;

/**
 @ingroup amx_llist
 @brief
 The linked list structure.
 */
typedef struct _amx_llist
{
	amx_llist_it_t *head; /**< Pointer to the first item in the linked list */
	amx_llist_it_t *tail; /**< Pointer to the last item in the linked list */
} amx_llist_t;

/**
 @ingroup amx_llist
 @brief
 Definition of the item delete function.

 A pointer to a delete function is used in the following functions @ref amx_llist_delete, @ref amx_llist_clean,
 @ref amx_llist_it_clean.
 */
typedef void (* amx_llist_it_delete_t) (amx_llist_it_t *it);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_TYPES_H__
