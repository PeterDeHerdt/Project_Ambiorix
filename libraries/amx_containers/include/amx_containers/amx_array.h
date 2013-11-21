#if !defined(__AMX_ARRAY_H__)
#define __AMX_ARRAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

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

typedef struct _amx_array_it amx_array_it_t;
typedef struct _amx_array amx_array_t;

struct _amx_array
{
	size_t items;            /**< Number of items in the array */
	size_t last_used;        /**< Highest used index */
	amx_array_it_t *buffer;  /**< The array buffer */
};

struct _amx_array_it
{
	amx_array_t *array;   /**< Pointer to the array */
	void *data;           /**< Pointer to the data */
};

typedef void (* amx_array_it_delete_t) (amx_array_it_t *it);

int amx_array_new(amx_array_t **array, const size_t items);
void amx_array_delete(amx_array_t **array, amx_array_it_delete_t func);

int amx_array_init(amx_array_t *array, const size_t items);
void amx_array_clean(amx_array_t *array, amx_array_it_delete_t func);

int amx_array_grow(amx_array_t *array, size_t items);
int amx_array_shrink(amx_array_t *array, size_t items, amx_array_it_delete_t func);

amx_array_it_t *amx_array_get_first(const amx_array_t *array);
amx_array_it_t *amx_array_get_last(const amx_array_t *array);

amx_array_it_t *amx_array_get_at(const amx_array_t *array, unsigned int index);
amx_array_it_t *amx_array_set_data_at(amx_array_t *array, unsigned int index, void *data);

amx_array_it_t *amx_array_it_get_next(const amx_array_it_t *reference);
amx_array_it_t *amx_array_it_get_previous(const amx_array_it_t *reference);

AMX_INLINE
void *amx_array_it_get_data(const amx_array_it_t *it)
{
	return it?it->data:NULL;
}

AMX_INLINE
size_t amx_array_size(const amx_array_t *array)
{
	return (array)?array->items:0;
}

AMX_INLINE
void *amx_array_get_data_at(const amx_array_t *array, unsigned int index)
{
	return amx_array_it_get_data(amx_array_get_at(array, index));
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_ARRAY_H__
