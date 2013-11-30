#if !defined(__AMX_LLIST_H__)
#define __AMX_LLIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define AMX_LLIST_RANGE UINT32_MAX

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
 Ambiorix linked list API header file
*/

/**
@ingroup amx_containers
@defgroup amx_llist Linked List
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
 Gets the data pointer from an linked list iterator
 */
#define amx_llist_it_get_data(it, type, member) ((type *)(((void *)it) - offsetof(type,member)))

/**
 @ingroup amx_llist
 @brief
 Definition of the linked list item delete function.

 A pointer to a delete function is used in the following functions @ref amx_llist_delete, @ref amx_llist_clean,
 @ref amx_llist_it_clean.
 */
typedef void (* amx_llist_it_delete_t) (amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Allocates a linked list.

 Allocates and initializes memory to store a linked list.
 This function allocates memory from the heap, if a linked list is on the stack, it can be initialized using
 function @ref amx_llist_init

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_llist_delete to free the memory

 @param llist a pointer to the location where the pointer to the new linked list can be stored

 @return
 -1 if an error occured. 0 on success
*/
int amx_llist_new(amx_llist_t **llist);

/**
 @ingroup amx_llist
 @brief
 Frees the previously allocated linked list

 Removes all items from the linked list, if a delete function is provided, it is called for each item
 after it was removed from the list.

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for linked lists that are allocated on the heap using @ref amx_llist_new

 @param llist a pointer to the location where the pointer to the linked list is be stored
 @param func pointer to a function that is called to free each item in the linked list
*/
void amx_llist_delete(amx_llist_t **llist, amx_llist_it_delete_t func);

/**
 @ingroup amx_llist
 @brief
 Initializes a linked list.

 Initializes the linked list structure. All pointers are reset to NULL.
 This function is typically called for linked lists that are on the stack.
 Allocating and initializing a linked list on the heap can be done using @ref amx_llist_new

 @note
 When calling this function on an already initialized linked list, that contains items, the linked list is reset
 and all items in the list are lost. Use @ref amx_llist_clean to remove all items from the list.

 @param llist a pointer to the linked list structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_llist_init(amx_llist_t *llist);

/**
 @ingroup amx_llist
 @brief
 Removes all items from the linked list

 Removes all items from the linked list, if a delete function is provided, it is called for each item
 after it was removed from the list.

 @param llist a pointer to the linked list structure
 @param func pointer to a function that is called to free each item in the linked list
*/
void amx_llist_clean(amx_llist_t *llist, amx_llist_it_delete_t func);

/**
 @ingroup amx_llist
 @brief
 Checks that the linked list is empty

 @param llist a pointer to the linked list structure

 @return
 returns true when the linked list contains no items, false when there is at least one item in the list.
*/
bool amx_llist_is_empty(const amx_llist_t *llist);

/**
 @ingroup amx_llist
 @brief
 Calculates the size of the linked list

 Loops over all items in the linked list and counts them.
 The size of the linked list is expressed in items.

 @note
 Use @ref amx_llist_is_empty to check if a list is empty, do not use this function to check if the list is empty

 @param llist a pointer to the linked list structure

 @return
 returns the number of items in linked list
*/
size_t amx_llist_size(const amx_llist_t *llist);

/**
 @ingroup amx_llist
 @brief
 Adds an item to the end of the linked list

 If the item is already in a list, it is removed from that list.

 @note
 Make sure that the iterator of the item is at least initialized when it is first used. Initializing an iterator can be
 done useing @ref amx_llist_it_init.
 An iterator that is already used in a linked list is considered initialized.

 @param llist a pointer to the linked list structure
 @param it a pointer to the linked list item iterator

 @return
 returns 0 when the item is added, -1 when there was an error
*/
int amx_llist_append(amx_llist_t *llist, amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Adds an item to the beginning of the linked list

 If the item is already in a list, it is removed from that list

 @note
 Make sure that the iterator of the item is at least initialized when it is first used. Initializing an iterator can be
 done useing @ref amx_llist_it_init.
 An iterator that is already used in a linked list is considered initialized.

 @param llist a pointer to the linked list structure
 @param it a pointer to the linked list item iterator

 @return
 returns 0 when the item is added, -1 when there was an error
*/
int amx_llist_prepend(amx_llist_t *llist, amx_llist_it_t *it);

/**
 @ingroup amx_llist
 @brief
 Gets an item at a certain position of the linked list

 This function is not removing the item from the linked list. To remove the item from the linked list
 use @ref amx_llist_take_at.

 @note
 This function loops over the linked list, counting each item until the desired index is reached.
 Avoid to use this function for big linked lists, it can have a huge impact on performance. Use @ref amx_llist_get_last
 to get the last item in the list and not this function.

 @param llist a pointer to the linked list structure
 @param index the position of the item to get

 @return
 Returns the iterator of the linked list item at the requested index, or NULL when there is no such item.
*/
amx_llist_it_t *amx_llist_get_at(amx_llist_t *llist, unsigned int index);

/**
 @ingroup amx_llist
 @brief
 Inserts an item at a certain position

 If the item is already in a list, it is removed from that list
 If the index is out of range (higher then number of items in the linked list) the item is not added.

 @note
 This function loops over the linked list, counting each item until the desired index is reached. The item is inserted
 before the found item. Avoid to use this function for big linked lists, it can have a huge impact on performance.
 Use @ref amx_llist_append to add an item at the end of the list and not this function.

 @param llist a pointer to the linked list structure
 @param index the position where the item must be inserted
 @param it a pointer to the linked list item iterator

 @return
 returns 0 when the item is added, -1 when there was an error
*/
int amx_llist_set_at(amx_llist_t *llist, unsigned int index, amx_llist_it_t *it);

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
unsigned int amx_llist_it_index_of(const amx_llist_it_t *it);

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

#endif // __AMX_LLIST_H__
