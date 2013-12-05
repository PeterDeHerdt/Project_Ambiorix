#if !defined(__AMX_LOG_MOCK_H__)
#define __AMX_LOG_MOCK_H__

#include <stdbool.h>
#include <dlfcn.h>

#define ck_mock_data_begin(fn) \
	typedef struct _Expectation_##fn {

#define ck_mock_data_add(field) field;

#define ck_mock_data_end(fn) \
} Expectation_##fn; \
void ck_mock_reset_##fn(void); \
bool ck_mock_verify_##fn(void); \
Expectation_##fn *ck_mock_next_expectation_##fn(void); \
Expectation_##fn *ck_mock_add_expectation_##fn(void); \

#define ck_mock_declare(fn, max) \
static size_t expectation_count_##fn; \
static size_t expectation_count_used_##fn; \
static Expectation_##fn expectations_##fn[max]; \
void ck_mock_reset_##fn(void) { \
	expectation_count_##fn = 0; \
	expectation_count_used_##fn = 0; \
} \
bool ck_mock_verify_##fn(void) { \
	return (expectation_count_##fn == expectation_count_used_##fn); \
} \
bool ck_mock_has_expectations_##fn(void) \
{ \
	return expectation_count_##fn!=0&&expectation_count_##fn!=expectation_count_used_##fn; \
} \
Expectation_##fn *ck_mock_next_expectation_##fn(void) \
{ \
	Expectation_##fn *exp = &expectations_##fn[expectation_count_used_##fn]; \
	if (expectation_count_used_##fn >= expectation_count_##fn) { \
		return exp; \
	} \
	expectation_count_used_##fn++; \
	return exp; \
} \
Expectation_##fn *ck_mock_add_expectation_##fn(void) \
{ \
	Expectation_##fn *exp = &expectations_##fn[expectation_count_##fn]; \
	expectation_count_##fn++; \
	return exp; \
} \

#define ck_mock_get_expectation(fn) (ck_mock_next_expectation_##fn())
#define ck_mock_add_expectation(fn) (ck_mock_add_expectation_##fn())
#define ck_mock_reset(fn) ck_mock_reset_##fn()
#define ck_mock_verify_expectations(fn) (ck_mock_verify_##fn())
#define ck_mock_has_expectations(fn) (ck_mock_has_expectations_##fn())

static inline void *mock_real_function(const char *lib, const char *name)
{
	void *handle = dlopen(lib, RTLD_LAZY);
	if (!handle) {
		return NULL;
	}

	return dlsym(handle,name);
}

#endif // __AMX_LOG_MOCK_H__
