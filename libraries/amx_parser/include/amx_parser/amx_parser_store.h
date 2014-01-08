/****************************************************************************
**
** - DISCLAIMER OF WARRANTY -
**
** THIS FILE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
** EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE SOURCE
** CODE IS WITH YOU. SHOULD THE SOURCE CODE PROVE DEFECTIVE, YOU
** ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
**
** - LIMITATION OF LIABILITY -
**
** IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
** WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES
** AND/OR DISTRIBUTES THE SOURCE CODE, BE LIABLE TO YOU FOR DAMAGES,
** INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
** ARISING OUT OF THE USE OR INABILITY TO USE THE SOURCE CODE
** (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED
** INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE
** OF THE SOURCE CODE TO OPERATE WITH ANY OTHER PROGRAM), EVEN IF SUCH
** HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGES.
**
****************************************************************************/

#if !defined(__AMX_PARSER_STORE_H__)
#define __AMX_PARSER_STORE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amx_containers.h>

/**
 @ingroup amx_parser
 @defgroup amx_parser_store Parser store
*/

/**
 @ingroup amx_parser_store
 @brief
 Default size of parser hash, the hash size will increase automatically.
*/
#define AMX_PARSER_STORE_DEFAULT_COUNT 5

/**
 @ingroup amx_parser_store
 @brief
 Function prototype for parsing a file
*/
typedef int (* amx_parser_store_parse_fn_t) (int fd, amx_var_t *result);
/**
 @ingroup amx_parser_store
 @brief
 Function prototype for verifying a file
*/
typedef int (* amx_parser_store_verify_fn_t) (int fd, bool *result);

/******************************** the store *********************************************/

typedef struct _amx_parser_store_parser {
    amx_htable_it_t hit;                    /**< Hash table iterator, can be used to store the variant in a hash table */
    char *name;                             /**< Name of the parser */
    amx_parser_store_parse_fn_t parse;      /**< Function pointer to the parse functionality */
    amx_parser_store_verify_fn_t verify;    /**< Function pointer to the verify functionality */
} amx_parser_store_parser_t;

/**
 @ingroup amx_parser_store
 @brief
 Initialize a parser context

 @note
 Use @ref amx_parser_store_parser_clean to clean up the context

 @param parser pointer to amx_parser_store_parser_t data structure
 @param name Name of the parser

 @return
 0 on success.
 -1 if an error has occured
*/
int  amx_parser_store_parser_init(amx_parser_store_parser_t *parser, const char *name);

/**
 @ingroup amx_parser_store
 @brief
 Cleanup a parser context

 @param parser pointer to amx_parser_store_parser_t data structure
 */
void amx_parser_store_parser_clean(amx_parser_store_parser_t *parser);

/**
 @ingroup amx_parser_store
 @brief
 Create a parser context

 @note
 Do not free the returned pointer yourself. Use @ref amx_parser_store_parser_delete to clean up the context.

 @param parser pointer to amx_parser_store_parser_t data structure
 @param name Name of the parser

 @return
 0 on success.
 -1 if an error has occured
*/
int  amx_parser_store_parser_new(amx_parser_store_parser_t **parser, const char *name);

/**
 @ingroup amx_parser_store
 @brief
 Cleanup and free a parser context

 @param parser pointer to amx_parser_store_parser_t data structure
 */
void amx_parser_store_parser_delete(amx_parser_store_parser_t **parser);

/**
 @ingroup amx_parser_store
 @brief
 Set the parse function for the specified parser context.

 @note
 Clearing the callback can be done by calling this function with a NULL pointer.

 @param parser pointer to amx_parser_store_parser_t data structure
 @param parse poiner to the parse functionality, can be NULL.

 @return
 0 on success.
 -1 if an error has occured
*/
int  amx_parser_store_parser_set_parse_function(amx_parser_store_parser_t *parser, amx_parser_store_parse_fn_t parse);

/**
 @ingroup amx_parser_store
 @brief
 Set the verify function for the specified parser context.

 @note
 Clearing the callback can be done by calling this function with a NULL pointer.

 @param parser pointer to amx_parser_store_parser_t data structure
 @param verify poiner to the verify functionality, can be NULL.

 @return
 0 on success.
 -1 if an error has occured
*/
int  amx_parser_store_parser_set_verify_function(amx_parser_store_parser_t *parser, amx_parser_store_verify_fn_t verify);

/**
 @ingroup amx_parser_store
 @brief
 Register a parser.

 @param parser pointer to amx_parser_store_parser_t data structure

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_register_parser( amx_parser_store_parser_t *parser);

/**
 @ingroup amx_parser_store
 @brief
 Unregister a parse

 @param parser pointer to amx_parser_store_parser_t data structure

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_unregister_parser( amx_parser_store_parser_t *parser);

/**
 @ingroup amx_parser_store
 @brief
 Parse the contents in the file descriptor.

 @param fd open file descriptor
 @param name the name of the parser to use
 @param result the result of the parse

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_parse_fd(int fd, const char *name, amx_var_t *result);

/**
 @ingroup amx_parser_store
 @brief
 Parse the contents in the file.

 @param file the file
 @param name the name of the parser to use
 @param result the result of the parse

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_parse_file(const char *file, const char *name, amx_var_t *result);

/**
 @ingroup amx_parser_store
 @brief
 Verify the contents in the file descriptor.

 @param fd open file descriptor
 @param name the name of the parser to use
 @param result the result: true if verify ok, false if an error occurred

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_verify_fd(int fd, const char *name, bool *result);

/**
 @ingroup amx_parser_store
 @brief
 Parse the contents in the file.

 @param file the file
 @param name the name of the parser to use
 @param result the result: true if verify ok, false if an error occurred

 @return
 0 on success.
 -1 if an error has occured
*/
int amx_parser_store_verify_file(const char *file, const char *name, bool *result);

#ifdef __cplusplus
}
#endif

#endif    // __AMX_PARSER_STORE_H__

