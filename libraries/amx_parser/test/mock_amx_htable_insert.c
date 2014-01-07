#include <stddef.h>
#include <stdio.h>
#include "mock.h"
#include "mock_amx_htable_insert.h"

#include <sys/types.h>
#include "amx_containers.h"

ck_mock_declare(amx_htable_insert,50)

#ifdef MOCK_MALLOC
typedef int (* amx_htable_insert_fn_t)(amx_htable_t *htable, const char *key, amx_htable_it_t *it);

int amx_htable_insert (amx_htable_t *htable, const char *key, amx_htable_it_t *it)
{
    if (ck_mock_has_expectations(amx_htable_insert))
    {
        if (ck_mock_get_expectation(amx_htable_insert)->fail)
        {
            return -1;
        }
    }

    amx_htable_insert_fn_t real_amx_htable_insert = mock_real_function("libamx_containers.so","amx_htable_insert");
    if (!real_amx_htable_insert) {
        return -1;
    }

    return real_amx_htable_insert(htable, key, it);
}
#endif
