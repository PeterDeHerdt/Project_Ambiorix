#if !defined(__AMX_LQUEUE_INLINE_H__)
#define __AMX_LQUEUE_INLINE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers/amx_lqueue_types.h>

static inline int amx_new_lqueue(amx_lqueue_t **lqueue)
{
	return amx_new_llist(lqueue);
}

static inline void amx_delete_lqueue(amx_lqueue_t **lqueue, amx_lqueue_it_delete_t func)
{
	amx_delete_llist(lqueue, func);
}

static inline int amx_init_lqueue(amx_lqueue_t *lqueue)
{
	return amx_init_llist(lqueue);
}

static inline void amx_clean_lqueue(amx_lqueue_t *lqueue, amx_lqueue_it_delete_t func)
{
	amx_clean_llist(lqueue, func);
}

static inline int amx_init_lqueue_it(amx_lqueue_it_t *it)
{
	return amx_init_llist_it(it);
}

static inline void amx_clean_lqueue_it(amx_llist_it_t *it, amx_llist_it_delete_t func)
{
	return amx_clean_llist_it(it, func);
}

static inline int amx_lqueue_add(amx_lqueue_t *lqueue, amx_lqueue_it_t *it)
{
	return amx_llist_append(lqueue, it);
}

static inline amx_lqueue_it_t *amx_lqueue_remove(amx_lqueue_t *lqueue)
{
	return amx_llist_take_first(lqueue);
}

static inline size_t amx_lqueue_size(amx_lqueue_t *lqueue)
{
	return amx_llist_size(lqueue);
}

#ifdef __cplusplus
}
#endif

#endif // AMX_LSTACK_INLINE_H
