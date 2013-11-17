#if !defined(__AMX_LLIST_IT_INLINE_H__)
#define __AMX_LLIST_IT_INLINE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist_types.h>

#if !defined(NO_DOXYGEN)
#define AMX_INLINE static inline
#else
/**
 @brief
 Helper macro for inlining functions
 */
#define AMX_INLINE
#endif

/**
 @file
 @brief
 Ambiorix linked list iterator API header file - inline functions
*/

/**
 @ingroup amx_llist
 @brief
 Gets the next iterator in the list

 This function does not remove the item from the linked list.

 @param reference a pointer to the linked list structure used as reference

 @return
 Returns the next iterator of the linked list, or NULL when there is not more item in the linked list.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_it_get_next(const amx_llist_it_t *reference)
{
	return reference?reference->next:NULL;
}

/**
 @ingroup amx_llist
 @brief
 Gets the previous iterator in the list

 This function does not remove the item from the linked list.

 @param reference a pointer to the linked list structure used as reference

 @return
 Returns the previous iterator of the linked list, or NULL when there is not more item in the linked list.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_it_get_previous(const amx_llist_it_t *reference)
{
	return reference?reference->prev:NULL;
}

/**
 @ingroup amx_llist
 @brief
 Checks that an iterator is in a list

 @param it a pointer to the linked list structure.

 @return
 true when the iterator is in the list, or false if it is not in a list
*/
AMX_INLINE
bool amx_llist_it_is_in_list(amx_llist_it_t *it)
{
	return (it && it->llist)?true:false;
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_IT_INLINE_IT_H__
