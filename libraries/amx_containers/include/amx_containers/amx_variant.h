#if !defined(__AMX_VARIANT_H__)
#define __AMX_VARIANT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <amx_containers/amx_llist.h>
#include <amx_containers/amx_htable.h>

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
 Ambiorix variant API header file
*/

/**
@ingroup amx_containers
@defgroup amx_var Variant
*/

#define AMX_VAR_TYPE_ID_VOID        0
#define AMX_VAR_TYPE_ID_STRING      1
#define AMX_VAR_TYPE_ID_INT8        2
#define AMX_VAR_TYPE_ID_INT16       3
#define AMX_VAR_TYPE_ID_INT32       4
#define AMX_VAR_TYPE_ID_INT64       5
#define AMX_VAR_TYPE_ID_UINT8       6
#define AMX_VAR_TYPE_ID_UINT16      7
#define AMX_VAR_TYPE_ID_UINT32      8
#define AMX_VAR_TYPE_ID_UINT64      9
#define AMX_VAR_TYPE_ID_FLOAT       10
#define AMX_VAR_TYPE_ID_DOUBLE      11
#define AMX_VAR_TYPE_ID_BOOL        12
#define AMX_VAR_TYPE_ID_LIST        13
#define AMX_VAR_TYPE_ID_HTABLE      14
#define AMX_VAR_TYPE_ID_FD          15

#define AMX_VAR_TYPE_ID_CUSTOM_BASE 16

#define AMX_VAR_TYPE_NAME_VOID      "amx_void"
#define AMX_VAR_TYPE_NAME_STRING    "amx_string"
#define AMX_VAR_TYPE_NAME_INT8      "amx_int8"
#define AMX_VAR_TYPE_NAME_INT16     "amx_int16"
#define AMX_VAR_TYPE_NAME_INT32     "amx_int32"
#define AMX_VAR_TYPE_NAME_INT64     "amx_int64"
#define AMX_VAR_TYPE_NAME_UINT8     "amx_uint8"
#define AMX_VAR_TYPE_NAME_UINT16    "amx_uint16"
#define AMX_VAR_TYPE_NAME_UINT32    "amx_uint32"
#define AMX_VAR_TYPE_NAME_UINT64    "amx_uint64"
#define AMX_VAR_TYPE_NAME_FLOAT     "amx_float"
#define AMX_VAR_TYPE_NAME_DOUBLE    "amx_double"
#define AMX_VAR_TYPE_NAME_BOOL      "amx_bool"
#define AMX_VAR_TYPE_NAME_FD        "amx_fd"

typedef struct _amx_var
{
	amx_llist_it_t lit;        /**< Linked list iterator, can be used to store the variant in a linked list */
	amx_htable_it_t hit;       /**< Hash table iterator, can be used to store the variant in a hash table */
	int type_id;               /**< Variant type */
	union 
	{
		char *s;               /**< String */
		int8_t i8;             /**< 8 bit signed integer */
		int16_t i16;           /**< 16 bit signed integer */
		int32_t i32;           /**< 32 bit signed integer */
		int64_t i64;           /**< 64 bit signed integer */
		uint8_t ui8;           /**< 8 bit unsigned integer */
		uint16_t ui16;         /**< 16 bit unsigned integer */
		uint32_t ui32;         /**< 32 bit unsigned integer */
		uint64_t ui64;         /**< 64 bit unsigned integer */
		float f;               /**< float */
		double d;              /**< double */
		bool b;                /**< boolean */
		amx_llist_t *vl;       /**< ambiorix linked list of variants */
		amx_htable_t *vm;      /**< ambiorix hash table (key - value pair) of variants */
		int fd;                /**< file descriptor */
		void *data;            /**< pointer to hold custom data types */
	} data;                    /**< Variant data */
} amx_var_t;

typedef int (* amx_var_copy_fn_t) (amx_var_t *dst, const amx_var_t *src);
typedef int (* amx_var_convert_fn_t) (amx_var_t *dest, const amx_var_t *src);
typedef int (* amx_var_compare_fn_t) (amx_var_t *var1, amx_var_t *var2);
typedef void (* amx_var_free_fn_t) (amx_var_t *var);

typedef struct _amx_var_type
{
	amx_var_copy_fn_t copy;        /**< Copy function */
	amx_var_convert_fn_t convert;  /**< Convert function */
	amx_var_compare_fn_t compare;  /**< Compare function */
	amx_var_free_fn_t del;         /**< Delete function */
	int type_id;                   /**< Type id - assigned when registered */
	amx_htable_it_t hit;           /**< Hash table iterator, used to store the type */
	const char name[];             /**< Unique name of the type */
} amx_var_type_t;

int amx_var_new(amx_var_t **var);
void amx_var_delete(amx_var_t **var);

int amx_var_init(amx_var_t *var);
void amx_var_clean(amx_var_t *var);

int amx_var_set_string_copy(amx_var_t *var, const char *string);
int amx_var_set_string_move(amx_var_t *var, char *string);
int amx_var_set_int8(amx_var_t *var, int8_t number);
int amx_var_set_int16(amx_var_t *var, int16_t number);
int amx_var_set_int32(amx_var_t *var, int32_t number);
int amx_var_set_int64(amx_var_t *var, int64_t number);
int amx_var_set_uint8(amx_var_t *var, uint8_t number);
int amx_var_set_uint16(amx_var_t *var, uint16_t number);
int amx_var_set_uint32(amx_var_t *var, uint32_t number);
int amx_var_set_uint64(amx_var_t *var, uint64_t number);
int amx_var_set_float(amx_var_t *var, float number);
int amx_var_set_double(amx_var_t *var, double number);
int amx_var_set_bool(amx_var_t *var, bool boolean);
int amx_var_set_llist_copy(amx_var_t *var, const amx_llist_t *list);
int amx_var_set_llist_move(amx_var_t *var, amx_llist_t *list);
int amx_var_set_htable_copy(amx_var_t *var, const amx_htable_t *table);
int amx_var_set_htable_move(amx_var_t *var, amx_htable_t *table);
int amx_var_set_fd(amx_var_t *var, int fd);
int amx_var_set_data_copy(amx_var_t *var, void *data, int type_id);
int amx_var_set_data_move(amx_var_t *var, void *data, int type_id);

char *amx_var_get_string(const amx_var_t *var);
const char *amx_var_get_string_da(const amx_var_t *var);
int8_t amx_var_get_int8(const amx_var_t *var);
int16_t amx_var_get_int16(const amx_var_t *var);
int32_t amx_var_get_int32(const amx_var_t *var);
int64_t amx_var_get_int64(const amx_var_t *var);
uint8_t amx_var_get_uint8(const amx_var_t *var);
uint16_t amx_var_get_uint16(const amx_var_t *var);
uint32_t amx_var_get_uint32(const amx_var_t *var);
uint64_t amx_var_get_uint64(const amx_var_t *var);
float amx_var_get_float(const amx_var_t *var);
double amx_var_get_double(const amx_var_t *var);
bool amx_var_get_bool(const amx_var_t *var);
amx_llist_t *amx_var_get_llist(const amx_var_t *var);
amx_llist_t *amx_var_get_llist_da(const amx_var_t *var);
amx_htable_t *amx_var_get_htable(const amx_var_t *var);
amx_htable_t *amx_var_get_htable_da(const amx_var_t *var);
int amx_var_get_fd(const amx_var_t *var);
void *amx_var_get_data_da(const amx_var_t *var);

amx_var_t *amx_var_from_llist_it(const amx_llist_it_t *it);
amx_var_t *amx_var_from_htable_it(const amx_htable_it_t *it);
amx_var_t *amx_var_from_array_it(const amx_array_it_t *it);

amx_llist_it_t *amx_var_get_llist_it(const amx_var_t *var);
amx_htable_it_t *amx_var_get_htable_it(const amx_var_t *var);

int amx_var_copy(amx_var_t *dest, const amx_var_t *src);
int amx_var_convert(amx_var_t *dst, const amx_var_t *src, int type_id);
int amx_var_compare(const amx_var_t *var1, const amx_var_t *var2);

int amx_var_get_type_id(const amx_var_t *var);
const char *amx_var_get_type_name(const amx_var_t *var);

int amx_var_register_type(amx_var_type_t *type);
int amx_var_unregister_type(amx_var_type_t *type);
const char *amx_var_get_type_name_from_id(int type_id);
int amx_var_get_type_id_from_name(const char *name);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LLIST_H__
