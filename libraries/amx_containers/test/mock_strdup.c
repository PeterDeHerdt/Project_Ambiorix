#include <stddef.h>

#include "mock.h"
#include "mock_strdup.h"

ck_mock_declare(strdup,50)

#ifdef MOCK_MALLOC
typedef char *(* strdup_fn_t)(const char *s);

char *strdup (const char *s)
{
	if (ck_mock_has_expectations(strdup))
	{
		if (ck_mock_get_expectation(strdup)->fail)
		{
			return NULL;
		}
	}

	strdup_fn_t real_strdup = mock_real_function("libc.so.6","strdup");
	if (!real_strdup) {
		return NULL;
	}

	return real_strdup(s);
}
#endif
