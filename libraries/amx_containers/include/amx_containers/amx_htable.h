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

#define amx_htable_it_get_data(it, type, member) ((type *)(((void *)it) - offsetof(type,member)))

#define AMX_HTABLE_RANGE UINT32_MAX

typedef struct _amx_htable_it amx_htable_it_t;

struct _amx_htable_it
{
	amx_array_it_t *ait;           /**< Pointer to position in the array */
	char *key;                     /**< The key */
	amx_htable_it_t *next;         /**< The next one if chained */
};

typedef unsigned int (* amx_htable_hash_func_t) (const char *key, unsigned int len);
typedef void (* amx_htable_it_delete_t) (const char *key, amx_htable_it_t *it);

typedef struct _amx_htable
{
	amx_array_t table;             /**< The hash table */
	size_t items;                  /**< Number of items in the hash tables */
	amx_htable_hash_func_t hfunc;  /**< The hash function */
	amx_htable_it_delete_t it_del; /**< The iterator delete function */
} amx_htable_t;

int amx_htable_new(amx_htable_t **htable, size_t reserve);
void amx_htable_delete(amx_htable_t **htable, amx_htable_it_delete_t func);

int amx_htable_init(amx_htable_t *htable, size_t reserve);
void amx_htable_clean(amx_htable_t *htable, amx_htable_it_delete_t func);

void amx_htable_set_hash_func(amx_htable_t *htable, amx_htable_hash_func_t func);
unsigned int amx_htable_key2index(const amx_htable_t *htable, const char *key);

bool amx_htable_is_empty(const amx_htable_t *htable);
size_t amx_htable_size(const amx_htable_t *htable);
size_t amx_htable_capacity(const amx_htable_t *htable);

int amx_htable_insert(amx_htable_t *htable, const char *key, amx_htable_it_t *it);
amx_htable_it_t *amx_htable_get(const amx_htable_t *htable, const char *key);
amx_htable_it_t *amx_htable_take(amx_htable_t *htable, const char *key);

amx_htable_it_t *amx_htable_get_first(const amx_htable_t *htable);
amx_htable_it_t *amx_htable_take_first(const amx_htable_t *htable);

bool amx_htable_contains(const amx_htable_t *htable, const char *key);

int amx_htable_it_init(amx_htable_it_t *it);
void amx_htable_it_clean(amx_htable_it_t *it);

amx_htable_it_t *amx_htable_it_get_next(const amx_htable_it_t *reference);
amx_htable_it_t *amx_htable_it_get_next_key(const amx_htable_it_t *reference);

void amx_htable_it_take(amx_htable_it_t *it);

const char *amx_htable_it_get_key(const amx_htable_it_t *it);

#ifdef __cplusplus
}
#endif

#endif // __AMX_HTABLE_H__
