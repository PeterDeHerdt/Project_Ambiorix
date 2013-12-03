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

/**
 @ingroup amx_rbuffer
 @brief
 The ring buffer structure.
 */
typedef struct _amx_rbuffer
{
	char *buffer_start;      /**< The beginning of the buffer */
	char *buffer_end;        /**< The end of the buffer */
	char *read_pos;          /**< Current read position */
	char *write_pos;         /**< Current write position */
} amx_rbuffer_t;

/**
 @ingroup amx_rbuffer
 @brief
 Allocates a ring buffer.

 Allocates and initializes memory to store a ring buffer. This functions allocates memory for the ring buffer structure
 as well as memory for the ring buffer itself.
 This function allocates memory from the heap, if a ring buffer structure is on the stack, it can be initialized using
 function @ref amx_rbuffer_init
 
 The size of the ring buffer is not fixed and can be changed with the functions @ref amx_rbuffer_grow or 
 @ref amx_rbuffer_shrink
 
 The size of the ring buffer is expressed in number of bytes that can be stored in the ring buffer.

 @note
 The allocated memory must be freed when not used anymore, use @ref amx_rbuffer_delete to free the memory of the ring
 buffer and the ring buffer structure and @ref amx_rbuffer_clean to free the buffer itself, but keep the ring buffer
 structure.

 @param rb a pointer to the location where the pointer to the new ring buffer structure can be stored
 @param size the size of the ring buffer in number of bytes

 @return
 -1 if an error occured. 0 on success
*/
int amx_rbuffer_new(amx_rbuffer_t **rb, size_t size);

/**
 @ingroup amx_rbuffer
 @brief
 Frees the previously allocated ring buffer

 Frees the ring buffer, all data that is still in the buffer will be lost. Also frees the allocated memory to store
 the ring buffer structure and sets the pointer to the structure to NULL.

 @note
 Only call this function for ring buffers that are allocated on the heap using @ref amx_rbuffer_new

 @param rb a pointer to the location where the pointer to the ring buffer is be stored
*/
void amx_rbuffer_delete(amx_rbuffer_t **rb);

/**
 @ingroup amx_rbuffer
 @brief
 Initializes a ring buffer.

 Initializes the ring buffer structure. 
 Memory is allocated from the heap to be able to store the number of bytes requested.

 This function is typically called for ring buffers that are on the stack.
 Allocating and initializing a ring buffer on the heap can be done using @ref amx_rbuffer_new

 @note
 When calling this function on an already allocated and initialized ring buffer a memory leak occurs, the previously
 allocated buffer is not reachable anymore.
 Use @ref amx_rbuffer_clean to free the buffer and reset all the pointers in the ring buffer structure to NULL.

 @param rb a pointer to the ring buffer structure.
 @param size the size of the ring buffer in number of bytes

 @return
 0 on success.or  -1 when error occured.
*/
int amx_rbuffer_init(amx_rbuffer_t *rb, size_t size);

/**
 @ingroup amx_rbuffer
 @brief
 Frees the buffer and sets all pointers of the ring buffer structure to NULL.

 @param rb a pointer to the ring buffer structure
*/
void amx_rbuffer_clean(amx_rbuffer_t *rb);

/**
 @ingroup amx_rbuffer
 @brief
 Grows the ring buffer

 Increases the capacity of the ring buffer with a number of bytes.
 The extra memory is always allocated behind the current position of the write pointer. Growing the ring buffer
 has no effect on the data that is already in the ring buffer.
 
 @param rb a pointer to the ring buffer structure
 @param size the number of bytes the ring buffer has to grow

 @return
 0 on success.
 -1 if an error has occured.
*/
int amx_rbuffer_grow(amx_rbuffer_t *rb, size_t size);

/**
 @ingroup amx_rbuffer
 @brief
 Shrinks the ring buffer

 Shrinks the ring buffer by the given number of bytes. The memory is freed.
 
 @note
 Shrinking the ring buffer could lead to data loss. The buffer shrinks from the current read pointer to the 
 write pointer. The data loss will always be the last part written in the ring buffer.

 @param rb a pointer to the ring buffer structure
 @param size the number of bytes the ring buffer has to shrink

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_rbuffer_shrink(amx_rbuffer_t *rb, size_t size);

/**
 @ingroup amx_rbuffer
 @brief
 Reads a number of bytes from the ring buffer
 
 Copies bytes from the ring buffer, starting from the current read posistion, into the provided buffer to a maximum
 number of bytes specified.
 When less bytes then the specified count are copied in the provided buffer, no more data is available in the ring buffer.
 
 @param rb a pointer to the ring buffer structure
 @param buf pointer to a buffer where the data can be copied in.
 @param count the maximum number of bytes that can be copied

 @return
 The number of bytes copied, when less then the specified maximum, the ring buffer is empty.
*/
ssize_t amx_rbuffer_read(amx_rbuffer_t *rb, void *buf, size_t count);

/**
 @ingroup amx_rbuffer
 @brief
 Writes a number of bytes to the ring buffer
 
 Copies the specified number of bytes to the ring buffer, starting from the current write posistion, 
 from the provided buffer.
 The ring buffer grows when there is not enough space left in the ring buffer.
 
 @param rb a pointer to the ring buffer structure
 @param buf pointer to a buffer that contains the data that must be put in the ring buffer.
 @param count the number of bytes that need to be copied in the ring buffer

 @return
 The number of bytes copied or -1 when failed to copy the bytes in the ring buffer.
*/
ssize_t amx_rbuffer_write(amx_rbuffer_t *rb, const void *buf, size_t count);

/**
 @ingroup amx_rbuffer
 @brief
 Get the size of the data stored in the ring buffer
 
 @param rb a pointer to the ring buffer structure

 @return
 The number of bytes stored in the ring buffer
*/
size_t amx_rbuffer_size(const amx_rbuffer_t *rb);

/**
 @ingroup amx_rbuffer
 @brief
 Get the capacity of the ring buffer
 
 The capacity is the maximum bytes that can be stored in the ring buffer.
 The capacity - the size is the number of bytes that is currently not used.
 
 @param rb a pointer to the ring buffer structure

 @return
 The number of bytes that can be stored in the ring buffer
*/
AMX_INLINE
size_t amx_rbuffer_capacity(const amx_rbuffer_t *rb)
{
	return rb?rb->buffer_end - rb->buffer_start:0;
}

/**
 @ingroup amx_rbuffer
 @brief
 Checks that the ring buffer is empty
 
 @param rb a pointer to the ring buffer structure

 @return
 true when there is no data in the ring buffer, or false when there is at least 1 byte of data stored in the ring buffer
*/
AMX_INLINE
bool amx_rbuffer_is_empty(const amx_rbuffer_t *rb)
{
	return rb?(rb->read_pos == rb->write_pos):true;
}

#ifdef __cplusplus
}
#endif

#endif // __AMX_RBUFFER_H__
