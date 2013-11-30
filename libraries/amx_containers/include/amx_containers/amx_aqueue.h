#if !defined(__AMX_AQUEUE_H__)
#define __AMX_AQUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_array.h>

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
 Ambiorix array based queue API header file
*/

/**
@ingroup amx_containers
@defgroup amx_aqueue Array Queue
*/

/**
 @ingroup amx_aqueue
 @brief
 The array queue structure.
 */
typedef amx_array_t amx_aqueue_t;

/**
 @ingroup amx_aqueue
 @brief
 The array queue iterator structure.
 */
typedef amx_array_it_t amx_aqueue_it_t;

/**
 @ingroup amx_aqueue
 @brief
 Definition of the item delete function.

 A pointer to a delete function is used in the following functions @ref amx_aqueue_delete, @ref amx_aqueue_clean
 */
typedef amx_array_it_delete_t amx_aqueue_it_delete_t;

/**
 @ingroup amx_aqueue
 @brief
 Allocates an array queue.

 Allocates and initializes memory to store an array queue.
 This function allocates memory from the heap, if an array queue is on the stack, it can be initialized using
 function @ref amx_aqueue_init

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_aqueue_delete to free the memory

 @param aqueue a pointer to the location where the pointer to the new array queue can be stored

 @return
 -1 if an error occured. 0 on success
*/
AMX_INLINE
int amx_aqueue_new(amx_aqueue_t **aqueue)
{
	return amx_array_new(aqueue, 10);
}

/**
 @ingroup amx_aqueue
 @brief
 Frees the previously allocated array queue

 Removes all items from the array queue, if a delete function is provided, it is called for each item in the queue.

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for array queues that are allocated on the heap using @ref amx_aqueue_new

 @param aqueue a pointer to the location where the pointer to the array queue is be stored
 @param func pointer to a function that is called to free each item in the array queue
*/
AMX_INLINE
void amx_aqueue_delete(amx_aqueue_t **aqueue, amx_aqueue_it_delete_t func)
{
	amx_array_delete(aqueue, func);
}

/**
 @ingroup amx_aqueue
 @brief
 Initializes a array queue.

 Initializes the array queue structure. All pointers are reset to NULL.
 This function is typically called for array queues that are on the stack.
 Allocating and initializing an array queue on the heap can be done using @ref amx_aqueue_new

 @note
 When calling this function on an already initialized array queue, that contains items, the array queue is reset
 and all items in the queue are lost. Use @ref amx_aqueue_clean to remove all items from the queue.

 @param aqueue a pointer to the array queue structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_aqueue_init(amx_aqueue_t *aqueue)
{
	return amx_array_init(aqueue, 10);
}

/**
 @ingroup amx_aqueue
 @brief
 Removes all items from the array queue

 Removes all items from the array queue, if a delete function is provided, it is called for each item in the queue

 @param aqueue a pointer to the array queue structure
 @param func pointer to a function that is called to free each item in the array queue
*/
AMX_INLINE
void amx_aqueue_clean(amx_aqueue_t *aqueue, amx_aqueue_it_delete_t func)
{
	amx_array_clean(aqueue, func);
}

/**
 @ingroup amx_aqueue
 @brief
 Adds data to the array queue

 @param aqueue a pointer to the array queue structure
 @param data a pointer to the data

 @return
 returns the iterator where the data is added, or NULL if adding the data failed
*/
AMX_INLINE
amx_aqueue_it_t *amx_aqueue_add(amx_aqueue_t *aqueue, void *data)
{
	return amx_array_append_data(aqueue, data);
}

/**
 @ingroup amx_aqueue
 @brief
 Removes the first added data from the queue

 @param aqueue a pointer to the array queue structure
 
 @return
 Pointer to the first added data or NULL if no more items on the queue
*/
AMX_INLINE
void *amx_aqueue_remove(amx_aqueue_t *aqueue)
{
	return amx_array_take_first_data(aqueue);
}

/**
 @ingroup amx_aqueue
 @brief
 Calculate the the number of items in the queue

 @param aqueue a pointer to the array queue structure
 
 @return
 The number of items in the array queue.
*/
AMX_INLINE
size_t amx_aqueue_size(const amx_aqueue_t *aqueue)
{
	return amx_array_size(aqueue);
}

/**
 @ingroup amx_aqueue
 @brief
 Checks that the array queue is empty

 @param aqueue a pointer to the array queue structure

 @return
 returns true when the array queue contains no items, false when there is at least one item in the queue.
*/
AMX_INLINE
size_t amx_aqueue_is_empty(const amx_aqueue_t *aqueue)
{
	return amx_array_is_empty(aqueue);
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_AQUEUE_H__
