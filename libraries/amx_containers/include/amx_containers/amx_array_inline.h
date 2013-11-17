#if !defined(__AMX_ARRAY_INLINE_H__)
#define __AMX_ARRAY_INLINE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_array_types.h>

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
 Ambiorix array API header file - inline functions
*/

AMX_INLINE
size_t amx_array_size(const amx_array_t *array)
{
	return (array)?array->items:0;
}

AMX_INLINE
const amx_array_it_t *amx_array_get_first(const amx_array_t *array)
{
	return amx_array_get_at(array, 0);
}

AMX_INLINE
const amx_array_it_t *amx_array_get_last(const amx_array_t *array)
{
	return amx_array_get_at(array, array?array->items - 1:0);
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_ARRAY_INLINE_H__
