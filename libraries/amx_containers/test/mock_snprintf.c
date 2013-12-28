#include <stdio.h>
#include <stdarg.h>

#include "mock.h"
#include "mock_snprintf.h"

ck_mock_declare(snprintf,50)

typedef char *(* snprintf_fn_t)(char *s, size_t n, const char *fmt, ...);

int snprintf(char *s, size_t n, const char *fmt, ...)
{
	if (ck_mock_has_expectations(snprintf))
	{
		if (ck_mock_get_expectation(snprintf)->fail)
		{
			return -1;
		}
	}

	va_list arguments;
	va_start ( arguments, fmt );
	int retval = vsnprintf(s, n, fmt, arguments);
	va_end ( arguments );

	return retval;
}
