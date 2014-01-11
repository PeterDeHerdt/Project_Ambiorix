#include <stdlib.h>
#include <unistd.h>

#include <amx_containers/amx_rbuffer.h>

#include "mock.h"
#include "mock_amx_rbuffer_write.h"

ck_mock_declare(amx_rbuffer_write,50)

typedef ssize_t (*amx_rbuffer_write_fn_t)(amx_rbuffer_t *rb, const void *buf, size_t count);

ssize_t amx_rbuffer_write(amx_rbuffer_t *rb, const void *buf, size_t count)
{
	if (ck_mock_has_expectations(amx_rbuffer_write))
	{
		if (ck_mock_get_expectation(amx_rbuffer_write)->fail)
		{
			return -1;
		}
	}

	amx_rbuffer_write_fn_t real_amx_rbuffer_write = mock_real_function("libamx_containers.so","amx_rbuffer_write");
	if (!real_amx_rbuffer_write) {
		return -1;
	}

	return real_amx_rbuffer_write(rb, buf, count);
}

