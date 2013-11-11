#if !defined(__AMX_LLIST_INLINE_H__)
#define __AMX_LLIST_INLINE_H__

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
 Ambiorix linked list API header file - inline functions
*/

/**
 @ingroup amx_llist
 @brief
 Gets the first item of the linked list

 This function does not remove the item from the linked list. To remove the item, use  @ref amx_llist_take_first.

 @param llist a pointer to the linked list structure

 @return
 Returns the first iterator of the linked list, or NULL when the linked list is empty.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_get_first(const amx_llist_t *llist)
{
	return llist?llist->head:NULL;
}

/**
 @ingroup amx_llist
 @brief
 Gets the last item of the linked list

 This function does not remove the item from the linked list. To remove the item, use  @ref amx_llist_take_last.

 @param llist a pointer to the linked list structure

 @return
 Returns the last iterator of the linked list, or NULL when the linked list is empty.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_get_last(const amx_llist_t *llist)
{
	return llist?llist->tail:NULL;
}

/**
 @ingroup amx_llist
 @brief
 Removes the first item from the linked list

 This function removes the item from the linked list. To get the item without removing it from the list
 use  @ref amx_llist_get_first.

 @param llist a pointer to the linked list structure

 @return
 Returns the first iterator of the linked list, or NULL when the linked list is empty.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_take_first(amx_llist_t *llist)
{
	amx_llist_it_t *it = amx_llist_get_first(llist);
	amx_llist_it_take(it);
	return it;
}

/**
 @ingroup amx_llist
 @brief
 Removes the last item from the linked list

 This function removes the item from the linked list. To get the item without removing it from the list
 use  @ref amx_llist_get_last.

 @param llist a pointer to the linked list structure

 @return
 Returns the last iterator of the linked list, or NULL when the linked list is empty.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_take_last(amx_llist_t *llist)
{
	amx_llist_it_t *it = amx_llist_get_last(llist);
	amx_llist_it_take(it);
	return it;
}

/**
 @ingroup amx_llist
 @brief
 Removes an item at a certain position of the linked list

 This function removes the item from the linked list. To get a pointer the item, without removing it, use
 @ref amx_llist_get_at.

 @note
 This function loops over the linked list, counting each item until the desired index is reached.
 Avoid to use this function for big linked lists, it can have a huge impact on performance. Use @ref amx_llist_take_last
 to remove the last item from the list and not this function.

 @param llist a pointer to the linked list structure
 @param index the position of the item to get

 @return
 Returns the iterator of the linked list item at the requested index, or NULL when there is no such item.
*/
AMX_INLINE
amx_llist_it_t *amx_llist_take_at(amx_llist_t *llist, unsigned int index)
{
	amx_llist_it_t *it = amx_llist_get_at(llist, index);
	amx_llist_it_take(it);
	return it;
}

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_INLINE_H__
