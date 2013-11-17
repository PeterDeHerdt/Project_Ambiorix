#if !defined(__AMX_ARRAY_TYPES_H__)
#define __AMX_ARRAY_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 @file
 @brief
 Ambiorix array types header file
*/

typedef struct _amx_array
{
	size_t items;         /**< Number of items in the array */
	size_t item_size;     /**< Size of an item */
	char *buffer;         /**< The array buffer */
} amx_array_t;

typedef struct _amx_array_it
{
	amx_array_t *array;   /**< Pointer to the array */
	char data[];          /**< The real data */
} amx_array_it_t;

#ifdef __cplusplus
}
#endif

#endif // __AMX_ARRAY_TYPES_H__
