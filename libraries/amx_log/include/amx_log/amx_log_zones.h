#if !defined(__AMX_LOG_ZONES_H__)
#define __AMX_LOG_ZONES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <amx_log/amx_log_core.h>

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
 Ambiorix logging zones API header file
*/

/**
 @ingroup amx_logging
 @defgroup amx_log_zones Log Zones
*/

/**
 @ingroup amx_log_zones
 @brief
 Opaque data stucture, containing the settings of a specific zone.
*/
typedef struct _amx_log_zone amx_log_zone_t;

/**
 @ingroup amx_log_zones
 @brief
 Enables a zone.

 Enables a zone and set the log level for that zone.

 When opening the logging facility, all zones are disabled by default. Log messages from a zone are not
 printed until the zone is enabled.

 @note
 When a zone was enabled previously, only the log level for that zone is modified.

 @param zone_name The name of the zone, any arbitray string can be used as zone name
 @param log_level The log level for that zone, logging messages of a log level higher are not printerd
*/
void amx_log_enable_zone(const char *zone_name, amx_log_level_t log_level);

/**
 @ingroup amx_log_zones
 @brief
 Disables a zone.

 After the zone is disabled, no more messages of that zone are logged.

 @note
 Calling this function for a zone that was not enabled before has no effect.

 @param zone_name The name of the zone
*/
void amx_log_disable_zone(const char *zone_name);

/**
 @ingroup amx_log_zones
 @brief
 Disables all zones.

 After this function returns no more messages for log zones are printed.
*/
void amx_log_disable_all_zones(void);

/**
 @ingroup amx_log_zones
 @brief
 Checks that the zone is enabled

 @param zone_name The name of the zone

 @return
 The function returns "true" when the given zone is enabled, "false" otherwhise
*/
bool amx_log_zone_is_enabled(const char *zone_name);

/**
 @ingroup amx_log_zones
 @brief
 Gets the first enabled log zone

 @note
 do not free the returned pointer, use @ref amx_log_disable_zone to remove the zone.

 @return
 The function returns a pointer to amx_log_zone_t data structure.
 The pointer must be treated as an opaque pointer.
*/
amx_log_zone_t *amx_log_first_zone(void);

/**
 @ingroup amx_log_zones
 @brief
 Gets the next enabled log zone

 @note
 do not free the returned pointer, use @ref amx_log_disable_zone to remove the zone

 @param reference pointer to amx_log_zone_t data structure, used as reference

 @return
 The function returns a pointer to amx_log_zone_t data structure, using the given pointer as a reference.
 If no next enabled log zone is available, the function returns a NULL pointer
*/
amx_log_zone_t *amx_log_next_zone(amx_log_zone_t *reference);

/**
 @ingroup amx_log_zones
 @brief
 Gets the pointer to amx_log_zone_t from a name.

 @note
 do not free the returned pointer, use @ref amx_log_disable_zone to remove the zone.

 @param zone_name the name of the zone for which a pointer to amx_log_zone_t must be returned

 @return
 The function returns a pointer to amx_log_zone_t data structure, using the given name as a reference.
 If no zone with the given name exists, the function returns a NULL pointer
*/
amx_log_zone_t *amx_log_get_zone(const char *zone_name);

/**
 @ingroup amx_log_zones
 @brief
 Gets zone name from a pointer to a amx_log_zone_t structure.

 @note
 do not free the returned pointer.

 @param zone pointer to amx_log_zone_t structure

 @return
 The name of the zone.
*/
const char *amx_log_get_zone_name(amx_log_zone_t *zone);

/**
 @ingroup amx_log_zones
 @brief
 Get the log level of a zone.

 @param zone pointer to amx_log_zone_t structure

 @return
 The log level of the zone.
*/
amx_log_level_t amx_log_get_zone_level(amx_log_zone_t *zone);

/**
 @ingroup amx_log_zones
 @brief
 Sets the log level of a zone.

 @param zone pointer to amx_log_zone_t structure
 @param log_level the log level
 */
void amx_log_set_zone_level(amx_log_zone_t *zone, amx_log_level_t log_level);

#ifdef __cplusplus
}
#endif

#endif // __AMX_LOG_ZONES_H__
