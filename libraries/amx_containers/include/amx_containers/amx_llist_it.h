#if !defined(__AMX_LLIST_INLINE_H__)
#define __AMX__LLIST_INLINE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist_types.h>

/**
 @file
 @brief
 Ambiorix linked list iterator API header file
*/

/**
 @ingroup amx_llist
 @brief
 Gets the data pointer from an linked list iterator
 */
#define amx_llist_it_get_data(it, type, member) ((type *)(((void *)it) - offsetof(type,member)))

/**
 @ingroup amx_llist
 @brief
 Initializes a linked list.iterator

 Initializes the linked list iterator structure. All pointers are reset to NULL.

 @note
 When calling this function on an already initialized linked list iterator, the linked list iterator is reset
 and the list the iterator was in is corrupted. Use @ref amx_llist_it_take to remove the iterator from the list or
 @ref amx_llist_it_clean to remove it from the list and clean up the iterator

 @param it a pointer to the linked list iterator structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_llist_it_init(amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Removes the iterator from the list and frees allocated memory

 If the iterator is in a list, it is removed from the list, when a delete function is provided, it is called to free
 up the memory.

 @note
 This function is not freeing the memory taken by the iterator, it uses a callback function for this.

 @param it a pointer to the linked list iterator structure
 @param func pointer to a function that is called to free the linked list item
*/
void amx_llist_it_clean(amx_llist_it_t *it, amx_llist_it_delete_t func);

/**
 @ingroup amx_llist
 @brief
 Removes the iterator from the list

 @param it a pointer to the linked list iterator structure
*/
void amx_llist_it_take(amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Inserts an iterator before another interator in the list

 If the iterator is in a list, it is removed from the list.

 @param reference a pointer to the linked list iterator structure that is used as reference
 @param it a pointer to the linked list iterator structure that needs to be inserted

 @return
 -1 if the reference iterator is not in a list.
 0 if the iterator is inserted
*/
int amx_llist_it_insert_before(amx_llist_it_t *reference, amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Inserts an iterator after another interator in the list

 If the iterator is in a list, it is removed from the list.

 @param reference a pointer to the linked list iterator structure that is used as reference
 @param it a pointer to the linked list iterator structure that needs to be inserted

 @return
 -1 if the reference iterator is not in a list.
 0 if the iterator is inserted
*/
int amx_llist_it_insert_after(amx_llist_it_t *reference, amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Gets the index of an iterator in the list

 @param it a pointer to the linked list iterator structure for which the index needs to be calculated

 @return
 The index of the iterator or AMX_LLIST_RANGE if the iterator is not in a list.
*/
size_t amx_llist_it_index_of(amx_llist_it_t *it);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_INLINE_H__
