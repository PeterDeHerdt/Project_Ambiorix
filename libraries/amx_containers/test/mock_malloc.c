#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "mock.h"
#include "mock_malloc.h"

ck_mock_declare(malloc,50)

#ifdef MOCK_MALLOC
extern void *dlcalloc(size_t nmemb, size_t size);
extern void *dlrealloc(void *ptr, size_t size);
extern void *dlmalloc(size_t size);
extern void dlfree(void *ptr);

void *calloc(size_t nmemb, size_t size)
{
	printf("==> MALLOC\n");
	if (ck_mock_has_expectations(malloc))
	{
		if (ck_mock_get_expectation(malloc)->fail)
		{
			printf("==> MALLOC FAIL\n");
			return NULL;
		}
	}
	return dlcalloc(nmemb, size);
}

void *realloc(void *ptr, size_t size) 
{
	printf("==> MALLOC\n");
	if (ck_mock_has_expectations(malloc))
	{
		if (ck_mock_get_expectation(malloc)->fail)
		{
			printf("==> MALLOC FAIL\n");
			return NULL;
		}
	}
	return dlrealloc(ptr, size);
}

void *malloc(size_t size)
{
	printf("==> MALLOC\n");
	if (ck_mock_has_expectations(malloc))
	{
		if (ck_mock_get_expectation(malloc)->fail)
		{
			printf("==> MALLOC FAIL\n");
			return NULL;
		}
	}

	void *ptr = dlmalloc(size);
	return ptr;
}

void free(void *ptr)
{
	dlfree(ptr);
}
#endif
