#if !defined(__AMX_ARRAY_FUNC_H__)
#define __AMX_ARRAY_FUNC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_array_types.h>

/**
 @file
 @brief
 Ambiorix array API header file
*/

#define amx_array_it_get_data(it, type) ((type *)(((void *)it) + sizeof(amx_array_it_t)))

int amx_array_new(amx_array_t **array, const size_t items, const size_t item_size);
void amx_array_delete(amx_array_t **array);

int amx_array_init(amx_array_t *array, const size_t items, const size_t item_size);
void amx_array_clean(amx_array_t *array);

int amx_array_grow(amx_array_t *array, size_t items);
int amx_array_shrink(amx_array_t *array, size_t items);

const amx_array_it_t *amx_array_get_at(const amx_array_t *array, unsigned int index);

const amx_array_it_t *amx_array_it_get_next(const amx_array_it_t *reference);
const amx_array_it_t *amx_array_it_get_previous(const amx_array_it_t *reference);

#ifdef __cplusplus
}
#endif

#endif // __AMX_ARRAY_FUNC_H__
