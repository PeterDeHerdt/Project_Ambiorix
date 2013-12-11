#if !defined(__AMX_ARRAY_H__)
#define __AMX_ARRAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

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
 Ambiorix array API header file
*/

/**
@ingroup amx_containers
@defgroup amx_array Array
*/


/**
 @ingroup amx_array
 @brief
 The array structure.
 */
typedef struct _amx_array
{
	size_t items;                  /**< Number of items in the array */
	size_t first_used;             /**< First used index */
	size_t last_used;              /**< Highest used index */
	struct _amx_array_it *buffer;  /**< The array buffer */
} amx_array_t;

/**
 @ingroup amx_array
 @brief
 The array iterator structure.
 */
typedef struct _amx_array_it
{
	amx_array_t *array;            /**< Pointer to the array */
	void *data;                    /**< Pointer to the data */
} amx_array_it_t;

/**
 @ingroup amx_array
 @brief
 Definition of the array item delete function.

 A pointer to a delete function is used in the following functions @ref amx_array_delete, @ref amx_array_clean,
 @ref amx_array_shrink, @ref amx_array_shift_right, @ref amx_array_shift_left.
 */
typedef void (* amx_array_it_delete_t) (amx_array_it_t *it);

/**
 @ingroup amx_array
 @brief
 Allocates an array.

 Allocates and initializes memory to store an array.
 This function allocates memory from the heap, if a array is on the stack, it can be initialized using
 function @ref amx_array_init
 
 The size of the array is not fixed and can be changed with the functions @ref amx_array_grow or 
 @ref amx_array_shrink
 
 The size of the array is expressed in number of items that can be stored in the array.
 All items in the allocated array are set to NULL.

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_array_delete to free the memory

 @param array a pointer to the location where the pointer to the new array can be stored
 @param items the size of the array in number of items

 @return
 -1 if an error occured. 0 on success
*/
int amx_array_new(amx_array_t **array, size_t items);

/**
 @ingroup amx_array
 @brief
 Frees the previously allocated array

 Removes all items from the array, if a delete function is provided, it is called for each used item in the array.

 Frees the allocated memory and sets the pointer to the array to NULL.

 @note
 Only call this function for arrays that are allocated on the heap using @ref amx_array_new

 @param array a pointer to the location where the pointer to the array is be stored
 @param func pointer to a function that is called to free each item in the array
*/
void amx_array_delete(amx_array_t **array, amx_array_it_delete_t func);

/**
 @ingroup amx_array
 @brief
 Initializes an array.

 Initializes the array structure. 
 Memory is allocated from the heap to be able to store the number of items requested.
 All the items are initialized to NULL.
 This function is typically called for arrays that are on the stack.
 Allocating and initializing an array on the heap can be done using @ref amx_array_new

 @note
 When calling this function on an already initialized array, that contains items, the array is reset
 and all items in the list are lost (reset to NULL). Use @ref amx_array_clean to remove all items from the list.

 @param array a pointer to the array structure.
 @param items the size of the array in number of items

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_array_init(amx_array_t *array, size_t items);

/**
 @ingroup amx_array
 @brief
 Removes all items from the array

 Removes all items from the array, if a delete function is provided, it is called for each used item in the array.

 @param array a pointer to the array structure
 @param func pointer to a function that is called to free each used item in the array
*/
void amx_array_clean(amx_array_t *array, amx_array_it_delete_t func);

/**
 @ingroup amx_array
 @brief
 Grows the array

 Grows the array by the given number of items. Extra memory is allocated to be able to store the number of items
 requested.

 @param array a pointer to the array structure
 @param items the number of items the array has to grow

 @return
 0 on success.
 -1 if an error has occured.
*/
int amx_array_grow(amx_array_t *array, size_t items);

/**
 @ingroup amx_array
 @brief
 Shrinks the array

 Shrinks the array by the given number of items. Memory is freed. If a delete function is provided it is called for each
 used item that is removed from the array.

 @param array a pointer to the array structure
 @param items the number of items the array has to shrink
 @param func pointer to a function that is called to free each used item in the array

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_array_shrink(amx_array_t *array, size_t items, amx_array_it_delete_t func);

/**
 @ingroup amx_array
 @brief
 Shift all items to the right in the array.

 Moves all items in the list to the right with the given number of items.
 Items that are moved out of the array are removed from the array. A function can be provided that is called for each
 used item that is removed from the array.
 
 E.g. Shifting the array to the right by 3 items, moves the item on index 0 to index 3, the item on index 1 to index 4, ...
 
 The items in the beginning of the array are reinitialized to NULL.

 @param array a pointer to the array structure
 @param items the number of items that needs to be shifted
 @param func pointer to a function that is called to free each used item in the array, that is moved out of the array.

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_array_shift_right(amx_array_t *array, size_t items, amx_array_it_delete_t func);

/**
 @ingroup amx_array
 @brief
 Shift all items to the left in the array.

 Moves all items in the list to the left with the given number of items.
 Items that are moved out of the array are removed from the array. A function can be provided that is called for each
 used item that is removed from the array.
 
 E.g. Shifting the array to the left by 3 items, moves the item on index 3 to index 0, the item on index 4 to index 1, ...
 
 The items at the end of the array are reinitialized to NULL.

 @param array a pointer to the array structure
 @param items the number of items that needs to be shifted
 @param func pointer to a function that is called to free each used item in the array, that is moved out of the array.

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_array_shift_left(amx_array_t *array, size_t items, amx_array_it_delete_t func);

/**
 @ingroup amx_array
 @brief
 Checks that the array is empty

 Ab array is empty if none of the items are used.
 
 @param array a pointer to the array structure

 @return
 returns true when the array contains no items that are used, false when there is at least one item used in the array.
*/
bool amx_array_is_empty(const amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Calculates the number of used items in the array

 Loops over all items in the array and counts the used items.
 The size of the array is expressed in used items.

 @note
 Use @ref amx_array_is_empty to check if an array is empty, do not use this function to check if there are used items
 in the array

 @param array a pointer to the array structure

 @return
 returns the number of used items in the array
*/
size_t amx_array_size(const amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Adds an item after the last used item in the array

 The array grows if the item can not be added to the array.

 @param array a pointer to the array structure
 @param data a pointer to the data that is added to the array

 @return
 returns the array iterator where the data is added, or NULL when adding the item fails.
*/
amx_array_it_t *amx_array_append_data(amx_array_t *array, void *data);

/**
 @ingroup amx_array
 @brief
 Adds an item before the first used item in the array

 The array grows if the item can not be added to the array.
 
 @note
 This function sets the pointer in the array. It is important that the pointer is valid as long as it is used in 
 the array. Before freeing the memory block, remove the pointer from the array.

 @param array a pointer to the array structure
 @param data a pointer to the data that is added to the array

 @return
 returns the array iterator where the data is added, or NULL when adding the item fails.
*/
amx_array_it_t *amx_array_prepend_data(amx_array_t *array, void *data);

/**
 @ingroup amx_array
 @brief
 Gets the item iterator for the given index

 @param array a pointer to the array structure
 @param index position in the array for which the iterator has to be returned

 @return
 returns the array iterator for the given index or NULL if the index is out of boundery.
*/
amx_array_it_t *amx_array_get_at(const amx_array_t *array, unsigned int index);

/**
 @ingroup amx_array
 @brief
 Sets data at the given index.

 @note
 This function sets the pointer in the array. It is important that the pointer is valid as long as it is used in 
 the array. Before freeing the memory block, remove the pointer from the array.

 @param array a pointer to the array structure
 @param index position in the array where the data has to be set
 @param data poiner to the data that has to be set in the array

 @return
 returns the array iterator for the given index or NULL if the index is out of boundery.
*/
amx_array_it_t *amx_array_set_data_at(amx_array_t *array, unsigned int index, void *data);

/**
 @ingroup amx_array
 @brief
 Gets the item iterator of the first used item in the array.

 @param array a pointer to the array structure

 @return
 returns the array iterator for the first used item in the array or NULL if there is no used item in the array.
*/
amx_array_it_t *amx_array_get_first(const amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Gets the item iterator of the last used item in the array.

 @param array a pointer to the array structure

 @return
 returns the array iterator for the last used item in the array or NULL if there is no used item in the array.
*/
amx_array_it_t *amx_array_get_last(const amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Takes the data pointer from the first used item in the array.

 @param array a pointer to the array structure

 @return
 returns the data pointer of the first used item in the array or NULL if there is no used item in the array.
*/
void *amx_array_take_first_data(amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Takes the data pointer from the last used item in the array.

 @param array a pointer to the array structure

 @return
 returns the data pointer of the last used item in the array or NULL if there is no used item in the array.
*/
void *amx_array_take_last_data(amx_array_t *array);

/**
 @ingroup amx_array
 @brief
 Gets the next used item in the array, starting from the provided array iterator.

 This function iterates forwards over all items starting from the reference iterator and searches the next used item.
 
 @param reference array iterator used as starting point.

 @return
 returns the iterator of the next used item in the array starting from the reference iterator, or NULL if there 
 are no more used items. 
*/
amx_array_it_t *amx_array_it_get_next(const amx_array_it_t *reference);

/**
 @ingroup amx_array
 @brief
 Gets the previous used item in the array, starting from the provided array iterator.

 This function iterates backwards over all items starting from the reference iterator and searches the previous
 used item.

 @param reference array iterator used as starting point.

 @return
 returns the iterator of the previous used item in the array starting from the reference iterator, or NULL if there 
 are no more used items. 
*/
amx_array_it_t *amx_array_it_get_previous(const amx_array_it_t *reference);

/**
 @ingroup amx_array
 @brief
 Gets the index of the iterator in the array

 @param it pointer to the array iterator.

 @return
 returns the index of the iterator.
*/
unsigned int amx_array_it_index(const amx_array_it_t *it);

/**
 @ingroup amx_array
 @brief
 Gets the capcity of the array.

 The capacity of an array is the number of items that can be stored in the array
 
 @param array pointer to the array structure

 @return
 returns the capacity of the array, expressed in number of items.
*/
AMX_INLINE
size_t amx_array_capacity(const amx_array_t *array)
{
	return (array)?array->items:0;
}

/**
 @ingroup amx_array
 @brief
 Gets the data pointer of the item at the given index.
 
 @param array pointer to the array structure
 @param index the index of the item

 @return
 returns the data pointer of the item at the given index or NULL if no data pointer was stored in that item or when
 the index is out of boundery.
*/
AMX_INLINE
void *amx_array_get_data_at(const amx_array_t *array, unsigned int index)
{
	amx_array_it_t *it = amx_array_get_at(array, index);
	return it?it->data:NULL;
}

/**
 @ingroup amx_array
 @brief
 Gets the data pointer of array iterator.
 
 @param it pointer to the item iterator

 @return
 returns the data pointer of iterator or NULL if no data pointer was stored in that iterator
*/
AMX_INLINE
void *amx_array_it_get_data(const amx_array_it_t *it)
{
	return it?it->data:NULL;
}

/**
 @ingroup amx_array
 @brief
 Sets the data pointer of an array iterator.

 @note
 This function sets the pointer in the array. It is important that the pointer is valid as long as it is used in 
 the array. Before freeing the memory block, remove the pointer from the array.

 @param it pointer to the item iterator
 @param data pointer to the data
*/
int amx_array_it_set_data(amx_array_it_t *it, void *data);

/**
 @ingroup amx_array
 @brief
 Gets and removes a data pointer from the iterator.

 This functions resets the data pointer of the iterator back to NULL.

 @param it pointer to the item iterator

 @return
 The data pointer of the iterator.
*/
void *amx_array_it_take_data(amx_array_it_t *it);

#ifdef __cplusplus
}
#endif

#endif // __AMX_ARRAY_H__
