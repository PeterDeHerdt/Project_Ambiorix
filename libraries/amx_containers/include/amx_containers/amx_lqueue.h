#if !defined(__AMX_LQUEUE_H__)
#define __AMX_LQUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_llist.h>

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
 Ambiorix linked queue API header file
*/

/**
@ingroup amx_containers
@defgroup amx_lqueue Linked Queue
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

 A pointer to a delete function is used in the following functions @ref amx_lqueue_delete, @ref amx_lqueue_clean
 */
typedef amx_llist_it_delete_t amx_lqueue_it_delete_t;

/**
 @ingroup amx_lqueue
 @brief
 Allocates a linked queue.

 Allocates and initializes memory to store a linked queue.
 This function allocates memory from the heap, if a linked queue is on the stack, it can be initialized using
 function @ref amx_lqueue_init

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_lqueue_delete to free the memory

 @param lqueue a pointer to the location where the pointer to the new linked queue can be stored

 @return
 -1 if an error occured. 0 on success
*/
AMX_INLINE
int amx_lqueue_new(amx_lqueue_t **lqueue)
{
	return amx_llist_new(lqueue);
}

/**
 @ingroup amx_lqueue
 @brief
 Frees the previously allocated linked queue

 Removes all items from the linked queue, if a delete function is provided, it is called for each item
 after it was removed from the queue

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for linked queues that are allocated on the heap using @ref amx_lqueue_new

 @param lqueue a pointer to the location where the pointer to the linked queue is be stored
 @param func pointer to a function that is called to free each item in the linked queue
*/
AMX_INLINE
void amx_lqueue_delete(amx_lqueue_t **lqueue, amx_lqueue_it_delete_t func)
{
	amx_llist_delete(lqueue, func);
}

/**
 @ingroup amx_lqueue
 @brief
 Initializes a linked queue.

 Initializes the linked queue structure. All pointers are reset to NULL.
 This function is typically called for linked queues that are on the stack.
 Allocating and initializing a linked queue on the heap can be done using @ref amx_lqueue_new

 @note
 When calling this function on an already initialized linked queue, that contains items, the linked queue is reset
 and all items in the queue are lost. Use @ref amx_lqueue_clean to remove all items from the queue.

 @param lqueue a pointer to the linked queue structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_lqueue_init(amx_lqueue_t *lqueue)
{
	return amx_llist_init(lqueue);
}

/**
 @ingroup amx_lqueue
 @brief
 Removes all items from the linked queue

 Removes all items from the linked queue, if a delete function is provided, it is called for each item
 after it was removed from the queue.

 @param lqueue a pointer to the linked queue structure
 @param func pointer to a function that is called to free each item in the linked queue
*/
AMX_INLINE
void amx_lqueue_clean(amx_lqueue_t *lqueue, amx_lqueue_it_delete_t func)
{
	amx_llist_clean(lqueue, func);
}

/**
 @ingroup amx_lqueue
 @brief
 Adds an item to the linked queue

 If the item is already in a queue, it is removed from that queue.

 @note
 Make sure that the iterator of the item is at least initialized when it is first used. Initializing an iterator can be
 done useing @ref amx_lqueue_it_init.
 An iterator that is already used in a linked queue is considered initialized.

 @param lqueue a pointer to the linked queue structure
 @param it a pointer to the linked queue item iterator

 @return
 returns 0 when the item is added, -1 when there was an error
*/
AMX_INLINE
int amx_lqueue_add(amx_lqueue_t *lqueue, amx_lqueue_it_t *it)
{
	return amx_llist_append(lqueue, it);
}

/**
 @ingroup amx_lqueue
 @brief
 Removes the first added item from the queue

 @param lqueue a pointer to the linked queue structure
 
 @return
 The iterator to the frist added item or NULL if no more items on the queue
*/
AMX_INLINE
amx_lqueue_it_t *amx_lqueue_remove(amx_lqueue_t *lqueue)
{
	return amx_llist_take_first(lqueue);
}

/**
 @ingroup amx_lqueue
 @brief
 Calculate the size of the queue, expressed in number of items

 @param lqueue a pointer to the linked queue structure
 
 @return
 The number of items on the linked queue.
*/
AMX_INLINE
size_t amx_lqueue_size(amx_lqueue_t *lqueue)
{
	return amx_llist_size(lqueue);
}

/**
 @ingroup amx_lqueue
 @brief
 Checks that the linked queue is empty

 @param lqueue a pointer to the linked queue structure

 @return
 returns true when the linked queue contains no items, false when there is at least one item on the queue.
*/
AMX_INLINE
size_t amx_lqueue_is_empty(amx_lqueue_t *lqueue)
{
	return amx_llist_is_empty(lqueue);
}

/**
 @ingroup amx_lqueue
 @brief
 Initializes a linked queue.iterator

 Initializes the linked queue iterator structure. All pointers are reset to NULL.

 @note
 When calling this function on an already initialized linked queue iterator, the linked queue iterator is reset
 and the queue the iterator was in is corrupted. Use @ref amx_lqueue_remove to remove the iterator from the queue

 @param it a pointer to the linked queue iterator structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_lqueue_it_init(amx_lqueue_it_t *it)
{
	return amx_llist_it_init(it);
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_LSTACK_H__
