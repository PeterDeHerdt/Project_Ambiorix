#if !defined(__AMX_RBUFFER_H__)
#define __AMX_RBUFFER_H__

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
 Ambiorix ring buffer API header file
*/

/**
@ingroup amx_containers
@defgroup amx_rbuffer Ring Buffer
*/

typedef struct _amx_rbuffer
{
	char *buffer_start;      /**< The beginning of the buffer */
	char *buffer_end;        /**< The end of the buffer */
	char *read_pos;          /**< Current read position */
	char *write_pos;         /**< Current write position */
} amx_rbuffer_t;

int amx_rbuffer_new(amx_rbuffer_t **rb, size_t size);
void amx_rbuffer_delete(amx_rbuffer_t **rb);

int amx_rbuffer_init(amx_rbuffer_t *rb, size_t size);
void amx_rbuffer_clean(amx_rbuffer_t *rb);

int amx_rbuffer_grow(amx_rbuffer_t *rb, size_t size);
int amx_rbuffer_shrink(amx_rbuffer_t *rb, size_t size);

ssize_t amx_rbuffer_read(amx_rbuffer_t *rb, void *buf, size_t count);
ssize_t amx_rbuffer_write(amx_rbuffer_t *rb, const void *buf, size_t count);

size_t amx_rbuffer_size(const amx_rbuffer_t *rb);
size_t amx_rbuffer_capacity(const amx_rbuffer_t *rb);
bool amx_rbuffer_is_empty(const amx_rbuffer_t *rb);

#ifdef __cplusplus
}
#endif

#endif // __AMX_RBUFFER_H__
