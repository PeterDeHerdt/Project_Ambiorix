#if !defined(__AMX_ASTACK_H__)
#define __AMX_ASTACK_H__

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
 Ambiorix array stack API header file
*/

/**
@ingroup amx_containers
@defgroup amx_astack Array stack
*/

/**
 @ingroup amx_astack
 @brief
 The array stack structure.
 */
typedef amx_array_t amx_astack_t;

/**
 @ingroup amx_astack
 @brief
 The array stack iterator structure.
 */
typedef amx_array_it_t amx_astack_it_t;

/**
 @ingroup amx_astack
 @brief
 Definition of the item delete function.

 A pointer to a delete function is used in the following functions @ref amx_astack_delete, @ref amx_astack_clean
 */
typedef amx_array_it_delete_t amx_astack_it_delete_t;

/**
 @ingroup amx_astack
 @brief
 Allocates a array stack.

 Allocates and initializes memory to store a array stack.
 This function allocates memory from the heap, if an array stack is on the stack, it can be initialized using
 function @ref amx_astack_init

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_astack_delete to free the memory

 @param astack a pointer to the location where the pointer to the new array stack can be stored

 @return
 -1 if an error occured. 0 on success
*/
AMX_INLINE
int amx_astack_new(amx_astack_t **astack)
{
	return amx_array_new(astack, 10);
}

/**
 @ingroup amx_astack
 @brief
 Frees the previously allocated array stack

 Removes all items from the array stack, if a delete function is provided, it is called for each item on the stack

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for array stacks that are allocated on the heap using @ref amx_astack_new

 @param astack a pointer to the location where the pointer to the array stack is be stored
 @param func pointer to a function that is called to free each item in the array stack
*/
AMX_INLINE
void amx_astack_delete(amx_astack_t **astack, amx_astack_it_delete_t func)
{
	amx_array_delete(astack, func);
}

/**
 @ingroup amx_astack
 @brief
 Initializes an array stack.

 Initializes the array stack structure. All pointers are reset to NULL.
 This function is typically called for array stacks that are on the stack.
 Allocating and initializing a array stack on the heap can be done using @ref amx_astack_new

 @note
 When calling this function on an already initialized array stack, that contains items, the array stack is reset
 and all items in the stack are lost. Use @ref amx_astack_clean to remove all items from the stack.

 @param astack a pointer to the array stack structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
AMX_INLINE
int amx_astack_init(amx_astack_t *astack)
{
	return amx_array_init(astack, 10);
}

/**
 @ingroup amx_astack
 @brief
 Removes all items from the array stack

 Removes all items from the array stack, if a delete function is provided, it is called for each item on the stack

 @param astack a pointer to the array stack structure
 @param func pointer to a function that is called to free each item in the array stack
*/
AMX_INLINE
void amx_astack_clean(amx_astack_t *astack, amx_astack_it_delete_t func)
{
	amx_array_clean(astack, func);
}

/**
 @ingroup amx_astack
 @brief
 Adds an item to the array stack

 If the item is already in a stack, it is removed from that stack.

 @param astack a pointer to the array stack structure
 @param data a pointer to the data that needs to be added to the stack

 @return
 returns 0 when the item is added, -1 when there was an error
*/
AMX_INLINE
amx_astack_it_t *amx_astack_push(amx_astack_t *astack, void *data)
{
	return amx_array_append_data(astack, data);
}

/**
 @ingroup amx_astack
 @brief
 Removes the last added data from the stack

 @param astack a pointer to the array stack structure
 
 @return
 The pointer to the data last added to the stack or NULL if no more items on the stack
*/
AMX_INLINE
void *amx_astack_pop(amx_astack_t *astack)
{
	return amx_array_take_last_data(astack);
}

/**
 @ingroup amx_astack
 @brief
 Calculate the number of items on the stack, expressed in number of items

 @param astack a pointer to the array stack structure
 
 @return
 The number of items on the array stack.
*/
AMX_INLINE
size_t amx_astack_size(const amx_astack_t *astack)
{
	return amx_array_size(astack);
}

/**
 @ingroup amx_astack
 @brief
 Checks that the array stack is empty

 @param astack a pointer to the array stack structure

 @return
 returns true when the array stack contains no items, false when there is at least one item on the stack.
*/
AMX_INLINE
bool amx_astack_is_empty(const amx_astack_t *astack)
{
	return amx_array_is_empty(astack);
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_ASTACK_H__
