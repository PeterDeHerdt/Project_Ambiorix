#if !defined(__AMX_LLIST_FUNC_H__)
#define __AMX_LLIST_FUNC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist_types.h>

/**
 @file
 @brief
 Ambiorix linked list API header file
*/

/**
 @ingroup amx_llist
 @brief
 Allocates a linked list.

 Allocates and initializes memory to store a linked list.
 This function allocates memory from the heap, if a linked list is on the stack, it can be initialized using
 function @ref amx_init_llist

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_delete_llist to free the memory

 @param llist a pointer to the location where the pointer to the new linked list can be stored

 @return
 -1 if an error occured. 0 on success
*/
int amx_new_llist(amx_llist_t **llist);

/**
 @ingroup amx_llist
 @brief
 Frees the previously allocated linked list

 Removes all items from the linked list, if a delete function is provided, it is called for each item
 after it was removed from the list.

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for linked lists that are allocated on the heap using @ref amx_new_llist

 @param llist a pointer to the location where the pointer to the linked stack is be stored
 @param func pointer to a function that is called to free each item in the linked list
*/
void amx_delete_llist(amx_llist_t **llist, amx_llist_it_delete_t func);

/**
 @ingroup amx_llist
 @brief
 Initializes a linked list.

 Initializes the linked list structure. All pointers are reset to NULL.
 This function is typically called for linked lists that are on the stack.
 Allocating and initializing a linked list on the heap can be done using @ref amx_new_llist

 @note
 When calling this function on an already initialized linked list, that contains items, the linked list is reset
 and all items in the list are lost. Use @ref amx_clean_llist to remove all items from the list.

 @param llist a pointer to the linked list structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_init_llist(amx_llist_t *llist);

/**
 @ingroup amx_llist
 @brief
 Removes all items from the linked list

 Removes all items from the linked list, if a delete function is provided, it is called for each item
 after it was removed from the list.

 @param llist a pointer to the linked list structure
 @param func pointer to a function that is called to free each item in the linked list
*/
void amx_clean_llist(amx_llist_t *llist, amx_llist_it_delete_t func);

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
 returns true when the linked list contains no items, false when there is at least one item in the list.
*/
size_t amx_llist_size(const amx_llist_t *llist);

/**
 @ingroup amx_llist
 @brief
 Adds an item to the end of the linked list

 If the item is already in a list, it is removed from that list.

 @note
 Make sure that the iterator of the item is at least initialized when it is first used. Initializing an iterator can be
 done useing @ref amx_init_llist_it.
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
 done useing @ref amx_init_llist_it.
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
int amx_llist_set_at(amx_llist_t *llist, size_t index, amx_llist_it_t *it);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_FUNC_H__
