#if !defined(__AMX_LSTACK_INLINE_H__)
#define __AMX_LSTACK_INLINE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_lstack_types.h>

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
 Ambiorix linked stack API header file - inline functions
*/

/**
 @ingroup amx_lstack
 @brief
 Allocates a linked stack.

 Allocates and initializes memory to store a linked stack.
 This function allocates memory from the heap, if a linked stack is on the stack, it can be initialized using
 function @ref amx_init_lstack

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_delete_lstack to free the memory

 @param lstack a pointer to the location where the pointer to the new linked stack can be stored

 @return
 -1 if an error occured. 0 on success
*/
AMX_INLINE
int amx_new_lstack(amx_lstack_t **lstack)
{
	return amx_new_llist(lstack);
}

/**
 @ingroup amx_lstack
 @brief
 Frees the previously allocated linked stack

 Removes all items from the linked stack, if a delete function is provided, it is called for each item
 after it was removed from the stack.

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for linked stacks that are allocated on the heap using @ref amx_new_lstack

 @param lstack a pointer to the location where the pointer to the linked stack is be stored
 @param func pointer to a function that is called to free each item in the linked stack
*/
AMX_INLINE
void amx_delete_lstack(amx_lstack_t **lstack, amx_lstack_it_delete_t func)
{
	amx_delete_llist(lstack, func);
}

/**
 @ingroup amx_lstack
 @brief
 Initializes a linked stack.

 Initializes the linked stack structure. All pointers are reset to NULL.
 This function is typically called for linked stacks that are on the stack.
 Allocating and initializing a linked stack on the heap can be done using @ref amx_new_lstack

 @note
 When calling this function on an already initialized linked stack, that contains items, the linked stack is reset
 and all items in the stack are lost. Use @ref amx_clean_lstack to remove all items from the stack.

 @param lstack a pointer to the linked stack structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_init_lstack(amx_lstack_t *lstack)
{
	return amx_init_llist(lstack);
}

/**
 @ingroup amx_lstack
 @brief
 Removes all items from the linked stack

 Removes all items from the linked stack, if a delete function is provided, it is called for each item
 after it was removed from the stack.

 @param lstack a pointer to the linked stack structure
 @param func pointer to a function that is called to free each item in the linked stack
*/
AMX_INLINE
void amx_clean_lstack(amx_lstack_t *lstack, amx_lstack_it_delete_t func)
{
	amx_clean_llist(lstack, func);
}

/**
 @ingroup amx_lstack
 @brief
 Adds an item to the linked stack

 If the item is already in a stack, it is removed from that stack.

 @note
 Make sure that the iterator of the item is at least initialized when it is first used. Initializing an iterator can be
 done useing @ref amx_init_lstack_it.
 An iterator that is already used in a linked stack is considered initialized.

 @param lstack a pointer to the linked stack structure
 @param it a pointer to the linked stack item iterator

 @return
 returns 0 when the item is added, -1 when there was an error
*/
AMX_INLINE
int amx_lstack_push(amx_lstack_t *lstack, amx_lstack_it_t *it)
{
	return amx_llist_append(lstack, it);
}

/**
 @ingroup amx_lstack
 @brief
 Removes the last added item from the stack

 @param lstack a pointer to the linked stack structure
 
 @return
 The iterator to the last added item or NULL if no more items on the stack
*/
AMX_INLINE
amx_lstack_it_t *amx_lstack_pop(amx_lstack_t *lstack)
{
	return amx_llist_take_last(lstack);
}

/**
 @ingroup amx_lstack
 @brief
 Calculate the size of the stack, expressed in number of items

 @param lstack a pointer to the linked stack structure
 
 @return
 The number of items on the linked stack.
*/
AMX_INLINE
size_t amx_lstack_size(amx_lstack_t *lstack)
{
	return amx_llist_size(lstack);
}

/**
 @ingroup amx_lstack
 @brief
 Checks that the linked stack is empty

 @param lstack a pointer to the linked stack structure

 @return
 returns true when the linked stack contains no items, false when there is at least one item on the stack.
*/
AMX_INLINE
bool amx_lstack_is_empty(amx_lstack_t *lstack)
{
	return amx_llist_is_empty(lstack);
}

/**
 @ingroup amx_lstack
 @brief
 Initializes a linked stack.iterator

 Initializes the linked stack iterator structure. All pointers are reset to NULL.

 @note
 When calling this function on an already initialized linked stack iterator, the linked stack iterator is reset
 and the stack the iterator was in is corrupted. Use @ref amx_lstack_pop to remove the iterator from the stack

 @param it a pointer to the linked stack iterator structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_init_lstack_it(amx_lstack_it_t *it)
{
	return amx_init_llist_it(it);
}

#ifdef __cplusplus
}
#endif

#endif // AMX_LSTACK_INLINE_H
