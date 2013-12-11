#if !defined(__AMX_HASH_H__)
#define __AMX_HASH_H__

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
 Ambiorix string hash functions header file
*/

/**
@ingroup amx_containers
@defgroup amx_hash Hash functions
*/

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 A simple hash function from Robert Sedgwicks Algorithms in C book.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_RS_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 A bitwise hash function written by Justin Sobel
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_JS_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 This hash algorithm is based on work by Peter J. Weinberger of AT&T Bell Labs.
 The book Compilers (Principles, Techniques and Tools) by Aho, Sethi and Ulman, recommends the use of hash functions 
 that employ the hashing methodology found in this particular algorithm.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_PJW_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 Similar to the PJW Hash function, but tweaked for 32-bit processors. Its the hash function widely used on most UNIX 
 systems.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_ELF_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 This hash function comes from Brian Kernighan and Dennis Ritchie's book "The C Programming Language". 
 It is a simple hash function using a strange set of possible seeds which all constitute a pattern of 31....31...31 etc,
 it seems to be very similar to the DJB hash function.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_BKDR_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 This is the algorithm of choice which is used in the open source SDBM project. The hash function seems to have a 
 good over-all distribution for many different data sets. It seems to work well in situations where there is a high 
 variance in the MSBs of the elements in a data set.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_SDBM_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 An algorithm produced by Professor Daniel J. Bernstein and shown first to the world on the usenet newsgroup 
 comp.lang.c. It is one of the most efficient hash functions ever published.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_DJB_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 A simple hash function from Robert Sedgwicks Algorithms in C book.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_DEK_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3, under the topic of sorting 
 and search chapter 6.4.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_BP_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 A simple hash function from Robert Sedgwicks Algorithms in C book.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_FNV_hash(const char* str, unsigned int len);

/**
 @ingroup amx_hash
 @brief
 Calculate a hash from a string
 
 An algorithm produced by Arash Partow. 
 He took ideas from all of the above hash functions making a hybrid rotative and additive hash function algorithm. 
 There isn't any real mathematical analysis explaining why one should use this hash function instead of the others 
 described above other than the fact that I tired to resemble the design as close as possible to a simple LFSR. 
 An empirical result which demonstrated the distributive abilities of the hash algorithm was obtained using a 
 hash-table with 100003 buckets, hashing The Project Gutenberg Etext of Webster's Unabridged Dictionary, the longest 
 encountered chain length was 7, the average chain length was 2, the number of empty buckets was 4579.
 
 @param str a string that needs to be hashed
 @param len the length that must be used of the string to calculate the hash
 
 @return
 The calculated hash value
 */
unsigned int amx_AP_hash(const char* str, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // __AMX_HASH_H__
