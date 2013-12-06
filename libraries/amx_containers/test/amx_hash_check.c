#include <stdlib.h>
#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_hash.h>

#include "mock_malloc.h"

static void generate_string(char *str, size_t length)
{
	const char *base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t len = strlen(base);
	for(unsigned int i =0; i < length; i++)
	{
		str[i] = base[rand() %  len];
	}
}

START_TEST(amx_hash_generation_check)
{
	char* key[] = { "abcdefghijklmnopqrstuvwxyz1234567890",
					"0987654321zyxwvutsrqponmlkjihgfedcba",
					"Damien",
					"Tom",
					"Peter",
					"Project Ambiorix",
					"1234567890",
					"abcdefghijklmnopqrstuvwxyz",
					"!@#$%^&*()_",
					"1+1=2",
					NULL
					};

	for(int i = 0; key[i]; i++) 
	{
		printf("Key:                         %s\n", key[i]);
		printf(" 1. RS-Hash Function Value:   %u\n",amx_RS_hash( key[i],strlen( key[i])));
		printf(" 2. JS-Hash Function Value:   %u\n",amx_JS_hash( key[i],strlen( key[i])));
		printf(" 3. PJW-Hash Function Value:  %u\n",amx_PJW_hash( key[i],strlen( key[i])));
		printf(" 4. ELF-Hash Function Value:  %u\n",amx_ELF_hash( key[i],strlen( key[i])));
		printf(" 5. BKDR-Hash Function Value: %u\n",amx_BKDR_hash( key[i],strlen( key[i])));
		printf(" 6. SDBM-Hash Function Value: %u\n",amx_SDBM_hash( key[i],strlen( key[i])));
		printf(" 7. DJB-Hash Function Value:  %u\n",amx_DJB_hash( key[i],strlen( key[i])));
		printf(" 8. DEK-Hash Function Value:  %u\n",amx_DEK_hash( key[i],strlen( key[i])));
		printf(" 9. BP-Hash Function Value:   %u\n",amx_BP_hash( key[i],strlen( key[i])));
		printf("10. FNV-Hash Function Value:  %u\n",amx_FNV_hash( key[i],strlen( key[i])));
		printf("11. AP-Hash Function Value:   %u\n",amx_AP_hash( key[i],strlen( key[i])));
	}
}
END_TEST

static unsigned int array[1003];
static unsigned int empty_buckets = 0;
static unsigned int max_chain_length = 0;

static void amx_hash_distribution_setup(void)
{
	memset(array, 0, sizeof(array));
	srand(2013);
	empty_buckets = 0;
	max_chain_length = 0;
}

static void amx_hash_distribution_teardown(void)
{
	for(int i = 0; i < 1003; i++) 
	{
		if (array[i] == 0)
		{
			empty_buckets++;
		}
		if (array[i] > max_chain_length)
		{
			max_chain_length = array[i];
		}
	}
	printf("===========================================\n");
	printf("Nr. of empty buckets = %d\n", empty_buckets);
	printf("max chain length     = %d\n", max_chain_length);
	printf("===========================================\n\n");
}

START_TEST(amx_hash_distibution_RS_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_RS_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nRS hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_JS_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_JS_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nJS hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_PJW_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_PJW_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nPJW hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_ELF_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_ELF_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nELF hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_BKDR_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_BKDR_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nBKDR hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_SBDM_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_SDBM_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nSBDM hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_DJB_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_DJB_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nDJB hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_DEK_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_DEK_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nDEK hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_BP_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_BP_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nBP hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_FNV_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_FNV_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nFNV hash algorithm\n");
}
END_TEST

START_TEST(amx_hash_distibution_AP_check)
{
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_AP_hash(key, 15) % 1003;
		array[hash]++;
	}

	printf("\nAP hash algorithm\n");
}
END_TEST

Suite *amx_hash_suite(void)
{
	Suite *s = suite_create ("amx_hash");
	TCase *tc = NULL;

	tc = tcase_create ("amx_hash_generation");
	tcase_add_test (tc, amx_hash_generation_check);
	suite_add_tcase (s, tc);

	tc = tcase_create ("amx_hash_distribution");
	tcase_add_checked_fixture (tc, amx_hash_distribution_setup, amx_hash_distribution_teardown);
	tcase_add_test (tc, amx_hash_distibution_RS_check);
	tcase_add_test (tc, amx_hash_distibution_JS_check);
	tcase_add_test (tc, amx_hash_distibution_PJW_check);
	tcase_add_test (tc, amx_hash_distibution_ELF_check);
	tcase_add_test (tc, amx_hash_distibution_BKDR_check);
	tcase_add_test (tc, amx_hash_distibution_SBDM_check);
	tcase_add_test (tc, amx_hash_distibution_DJB_check);
	tcase_add_test (tc, amx_hash_distibution_DEK_check);
	tcase_add_test (tc, amx_hash_distibution_BP_check);
	tcase_add_test (tc, amx_hash_distibution_FNV_check);
	tcase_add_test (tc, amx_hash_distibution_AP_check);
	suite_add_tcase (s, tc);

	return s;
}
