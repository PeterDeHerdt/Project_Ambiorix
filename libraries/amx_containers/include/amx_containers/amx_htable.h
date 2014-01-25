#if !defined(__AMX_HTABLE_H__)
#define __AMX_HTABLE_H__

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
 Ambiorix hash table API header file
*/

/**
@ingroup amx_containers
@defgroup amx_htable Hash Table
*/

/**
 @ingroup amx_htable
 @brief
 Gets the data pointer from an hash table iterator
 */
#define amx_htable_it_get_data(it, type, member) ((type *)(((void *)it) - offsetof(type,member)))

/** 
 @ingroup amx_llist
 @brief
 Loops over the list
 
 Iterates over the list and updates the iterator each time.

 @warning
 Do not modify the list itself while using this macro.
*/
#define amx_htable_for_each(it, htable) for(amx_htable_it_t *it = amx_htable_get_first(htable); \
                                         it; \
                                         it = amx_htable_it_get_next(it))

/**
 @ingroup amx_htable
 @brief
 Out of range indicator
 */
#define AMX_HTABLE_RANGE UINT32_MAX

typedef struct _amx_htable_it amx_htable_it_t;

/**
 @ingroup amx_htable
 @brief
 The hash table iterator structure.
 */
struct _amx_htable_it
{
	amx_array_it_t *ait;           /**< Pointer to position in the array */
	char *key;                     /**< The key */
	amx_htable_it_t *next;         /**< The next one if chained */
};

/**
 @ingroup amx_htable
 @brief
 Definition of the hash function.

 The hash function calculates a hash from the key. A custom hash function can be implemented and set
 on the hash table using @ref amx_htable_set_hash_func. The calculated hash is used to calculate the index of the 
 position of the key in the array.
 */
typedef unsigned int (* amx_htable_hash_func_t) (const char *key, unsigned int len);

/**
 @ingroup amx_htable
 @brief
 Definition of the hash table item delete function.

 A pointer to a delete function is used in the following functions @ref amx_htable_delete, @ref amx_htable_clean,
 @ref amx_htable_it_clean.
 */
typedef void (* amx_htable_it_delete_t) (const char *key, amx_htable_it_t *it);

/**
 @ingroup amx_htable
 @brief
 The hash table structure.
 */
typedef struct _amx_htable
{
	amx_array_t table;             /**< The hash table */
	size_t items;                  /**< Number of items in the hash tables */
	amx_htable_hash_func_t hfunc;  /**< The hash function */
	amx_htable_it_delete_t it_del; /**< The iterator delete function */
} amx_htable_t;

/**
 @ingroup amx_htable
 @brief
 Allocates a hash table.

 Allocates and initializes memory to store a hash table.
 This function allocates memory from the heap, if a hash table is on the stack, it can be initialized using
 function @ref amx_htable_init

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_htable_delete to free the memory

 @param htable a pointer to the location where the pointer to the new hash table can be stored
 @param reserve Number of items that needs to be reserved

 @return
 -1 if an error occured. 0 on success
*/
int amx_htable_new(amx_htable_t **htable, size_t reserve);

/**
 @ingroup amx_htable
 @brief
 Frees the previously allocated hash table

 Removes all items from the hash table, if a delete function is provided, it is called for each item
 after it was removed from the hash table.

 Frees the allocated memory and sets the pointer to NULL.

 @note
 Only call this function for hash tables that are allocated on the heap using @ref amx_htable_new

 @param htable a pointer to the location where the pointer to the hash table is be stored
 @param func pointer to a function that is called to free each item in the hash table
*/
void amx_htable_delete(amx_htable_t **htable, amx_htable_it_delete_t func);

/**
 @ingroup amx_htable
 @brief
 Initializes a hash table.

 Initializes the hash table structure. All pointers are reset to NULL.
 This function is typically called for hash tables that are on the stack.
 Allocating and initializing a hash table on the heap can be done using @ref amx_htable_new

 @note
 When calling this function on an already initialized hash table, that contains items, the hash table is reset
 and all items in the table are lost. Use @ref amx_htable_clean to remove all items from the table.

 @param htable a pointer to the hash table structure.
 @param reserve Number of items that needs to be reserved

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_htable_init(amx_htable_t *htable, size_t reserve);

/**
 @ingroup amx_htable
 @brief
 Removes all items from the hash table

 Removes all items from the hash table, if a delete function is provided, it is called for each item
 after it was removed from the table. The buffer used to store the items is freed as well.

 @param htable a pointer to the htable structure
 @param func pointer to a function that is called to free each item in the hash table
*/
void amx_htable_clean(amx_htable_t *htable, amx_htable_it_delete_t func);

/**
 @ingroup amx_htable
 @brief
 Sets the hash function for the hash table

 A custom implementation for hash calculation function can be set on the hash table.
 By default the @ref amx_BKDR_hash function is set to calculate the hashes. Other hash functions are available,
 see @ref amx_hash
 
 @param htable a pointer to the htable structure
 @param func pointer to a function that is called for each hash that needs be calculated
*/
void amx_htable_set_hash_func(amx_htable_t *htable, amx_htable_hash_func_t func);

/**
 @ingroup amx_htable
 @brief
 Converts a key into an index

 This function converts a key into an index using the hash function and the reserved size of the table
 
 @param htable a pointer to the htable structure
 @param key the key for which an index must be calculated
 
 @return
 The calculated index or @ref AMX_HTABLE_RANGE if no reserved items are available in the table
*/
unsigned int amx_htable_key2index(const amx_htable_t *htable, const char *key);

/**
 @ingroup amx_htable
 @brief
 Checks that the hahs table is empty

 @param htable a pointer to the hash table structure

 @return
 returns true when the hash table contains no items, false when there is at least one item in the table.
*/
AMX_INLINE
bool amx_htable_is_empty(const amx_htable_t *htable)
{
	return htable?(htable->items == 0):true;
}

/**
 @ingroup amx_htable
 @brief
 Calculates the size of the hash table

 The size of the hash table is expressed in items.

 @note
 Use @ref amx_htable_is_empty to check if a hash table is empty, do not use this function to check if the hash table
 is empty

 @param htable a pointer to the hash table structure

 @return
 returns the number of items in hash table
*/
AMX_INLINE
size_t amx_htable_size(const amx_htable_t *htable)
{
	return htable?htable->items:0;
}

/**
 @ingroup amx_htable
 @brief
 Calculates the capacity of the hash table

 The capacity is the number of items that is reserved

 @param htable a pointer to the hash table structure

 @return
 returns the number of reserved item positions in hash table
*/
AMX_INLINE
size_t amx_htable_capacity(const amx_htable_t *htable)
{
	return htable?htable->table.items:0;
}

/**
 @ingroup amx_htable
 @brief
 Inserts an item in the hash table.

 The key provided is converted into an index using @ref amx_htable_key2index.
 When an position is already taken the items on that position are chained.
 The same key can be used for different items.
 When the number of items in the hash table is above 75% of the capacity, the table grows. The number of items
 that are reserved will be doubled with a maximum of 1024 items each time.

 @param htable a pointer to the hash table structure
 @param key the item key
 @param it the item htable iterator

 @return
 0 when the item is inserted, -1 when an error occured
*/
int amx_htable_insert(amx_htable_t *htable, const char *key, amx_htable_it_t *it);

/**
 @ingroup amx_htable
 @brief
 Gets a hash table iterator from the hash table.

 The key provided is converted into an index using @ref amx_htable_key2index.
 If multiple items are available at the calculated positon with the same key, the first one is returned.
 The next item can be retrieved with @ref amx_htable_it_get_next or with @ref amx_htable_it_get_next_key if it must
 have the same key.
 
 @note
 The item is not removed from the hash table. to remove the item use @ref amx_htable_take

 @param htable a pointer to the hash table structure
 @param key the item key

 @return
 Pointer to the hash table iterator or NULL if no iterator was stored with the given key.
 The pointer can be converted to the real data pointer using @ref amx_htable_it_get_data
*/
amx_htable_it_t *amx_htable_get(const amx_htable_t *htable, const char *key);

/**
 @ingroup amx_htable
 @brief
 Removes a hash table iterator from the hash table.

 The key provided is converted into an index using @ref amx_htable_key2index.
 If multiple items are available at the calculated positon with the same key, the first one is removed and returned.
 
 @note
 The item is removed from the hash table. To fetch the next item in the hash table with the same key, call this
 function again.

 @param htable a pointer to the hash table structure
 @param key the item key

 @return
 Pointer to the hash table iterator or NULL if no iterator was stored with the given key.
 The pointer can be converted to the real data pointer using @ref amx_htable_it_get_data
*/
amx_htable_it_t *amx_htable_take(amx_htable_t *htable, const char *key);

/**
 @ingroup amx_htable
 @brief
 Gets the first item stored in the table.

 This function iterates the table from the beginning and returns the first one encounter.

 @note
 The order of the items in the table is not the same as the order they were inserted in the table. So the first
 item found can be different then the first item inserted.

 @param htable a pointer to the hash table structure

 @return
 Pointer to the hash table iterator or NULL if no items were stored in the hash table.
*/
amx_htable_it_t *amx_htable_get_first(const amx_htable_t *htable);

/**
 @ingroup amx_htable
 @brief
 Verifies that a key is in the hash table

 @param htable a pointer to the hash table structure
 @param key the key 

 @return
 True when the key is at least once found in the table, false otherwise.
*/
AMX_INLINE
bool amx_htable_contains(const amx_htable_t *htable, const char *key)
{
	return amx_htable_get(htable, key)?true:false;
}

/**
 @ingroup amx_htable
 @brief
 Initializes a hash table.iterator

 Initializes the hash table iterator structure. All pointers are reset to NULL.

 @note
 When calling this function on an already initialized hash table iterator, the hash table iterator is reset and
 data can be lost. Use @ref amx_htable_it_clean to remove the iterator from the table ant to clean up the iterator.
 
 @param it a pointer to the hash table iterator structure.

 @return
 0 on success.
 -1 if a NULL pointer is given.
*/
int amx_htable_it_init(amx_htable_it_t *it);

/**
 @ingroup amx_htable
 @brief
 Removes the iterator from the htable and frees allocated memory

 If the iterator is in a hash table, it is removed from the table, when a delete function is provided, it is called to free
 up the memory.

 @note
 This function is not freeing the memory taken by the iterator, it uses a callback function for this.

 @param it a pointer to the hash table iterator structure
 @param func pointer to a function that is called to free the hash table item
*/
void amx_htable_it_clean(amx_htable_it_t *it, amx_htable_it_delete_t func);

/**
 @ingroup amx_htable
 @brief
 Gets the next iterator in the hash table

 Searches the next hash table iterator in the hash table, using the provided iterator as a reference.
 The iterator returned can have another key as the reference provided. 
 
 @param reference a pointer to the hash table iterator structure

 @return
 The next iterator in the hash table, starting from the reference or NULL if the reference was the last in the table.
*/
amx_htable_it_t *amx_htable_it_get_next(const amx_htable_it_t *reference);

/**
 @ingroup amx_htable
 @brief
 Gets the next iterator in the hash table with the same key

 Searches the next hash table iterator in the hash table, using the provided iterator as a reference.
 The iterator returned has the same key as the reference provided. 
 
 @param reference a pointer to the hash table iterator structure

 @return
 The next iterator in the hash table with the same key, starting from the reference or 
 NULL if the reference was the last in the table with that key.
*/
amx_htable_it_t *amx_htable_it_get_next_key(const amx_htable_it_t *reference);

/**
 @ingroup amx_htable
 @brief
 Removes the iterator from the hash table

 This function only removes the iterator rom the hash table, it does not clean any memory allocated for the
 iterator.
 
 @param it a pointer to the hash table iterator structure
 */
void amx_htable_it_take(amx_htable_it_t *it);

/**
 @ingroup amx_htable
 @brief
 Gets the key from the iterator

 When an iterator is in a hash table, it has a key set.
 An iterator that is not in a list can have a key set.
 
 @param it a pointer to the hash table iterator structure
 
 @return
 the key of the iterator or NULL if the iterator has no key
 */
AMX_INLINE
const char *amx_htable_it_get_key(const amx_htable_it_t *it)
{
	return it?it->key:NULL;
}

/**
 @ingroup amx_htable
 @brief
 Removes the first item stored in the table.

 This function iterates the table from the beginning and removes and returns the first one encounter.

 @note
 The order of the items in the table is not the same as the order they were inserted in the table. So the first
 item found can be different then the first item inserted.

 @param htable a pointer to the hash table structure

 @return
 Pointer to the hash table iterator or NULL if no items were stored in the hash table.
*/
AMX_INLINE
amx_htable_it_t *amx_htable_take_first(const amx_htable_t *htable)
{
	amx_htable_it_t *it = amx_htable_get_first(htable);
	amx_htable_it_take(it);
	return it;
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_HTABLE_H__
