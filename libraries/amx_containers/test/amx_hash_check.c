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

static unsigned int array[2006];
static bool first_double = false;

static void amx_hash_distribution_setup(void)
{
	memset(array, 0, sizeof(array));
	srand(2013);
	first_double = false;
	printf("\n===========================================\n");
}

static void amx_hash_distribution_teardown(void)
{
	unsigned int empty_buckets = 0;
	unsigned int unique_buckets = 0;
	unsigned int max_chain_length = 0;
	unsigned int number_of_chains = 0;
	unsigned int total_chain_lengths = 0;
	for(int i = 0; i < 2006; i++) 
	{
		if (array[i] == 0)
		{
			empty_buckets++;
		}
		if (array[i] > max_chain_length)
		{
			max_chain_length = array[i];
		}
		if (array[i] > 1) 
		{
			number_of_chains++;
			total_chain_lengths+=array[i];
		}
		if (array[i] == 1)
		{
			unique_buckets++;
		}
	}
	printf("Nr. of empty buckets   = %d\n", empty_buckets);
	printf("Nr. of chained buckets = %d\n", number_of_chains);
	printf("Nr. of uinque buckets  = %d\n", unique_buckets);
	printf("Total buckets          = %d\n", empty_buckets + number_of_chains + unique_buckets);
	printf("max chain length       = %d\n", max_chain_length);
	printf("average chain length   = %f\n", (double)(total_chain_lengths / number_of_chains));
	printf("===========================================\n");
}

START_TEST(amx_hash_distibution_RS_check)
{
	printf("RS hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_RS_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}
}
END_TEST

START_TEST(amx_hash_distibution_JS_check)
{
	printf("JS hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_JS_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_PJW_check)
{
	printf("PJW hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_PJW_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_ELF_check)
{
	printf("ELF hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_ELF_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_BKDR_check)
{
	printf("BKDR hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_BKDR_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_SBDM_check)
{
	printf("SBDM hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_SDBM_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_DJB_check)
{
	printf("DJB hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_DJB_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_DEK_check)
{
	printf("DEK hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_DEK_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_BP_check)
{
	printf("BP hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_BP_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_FNV_check)
{
	printf("FNV hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_FNV_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}

}
END_TEST

START_TEST(amx_hash_distibution_AP_check)
{
	printf("AP hash algorithm\n");
	
	for(int i = 0; i < 2006; i++)
	{
		char key[15];
		generate_string(key, 15);
		unsigned int hash = amx_AP_hash(key, 15) % 2006;
		if (array[hash] && !first_double)
		{
			first_double = true;
			printf("First double insertion = %d\n", i);
		}
		array[hash]++;
	}
}
END_TEST

START_TEST(amx_hash_BKDR_hashes_check)
{
	printf("[Key1] = %d\n", amx_BKDR_hash("Key1",4) % 64);
	printf("[Key2] = %d\n", amx_BKDR_hash("Key2",4) % 64);
	printf("[Key3] = %d\n", amx_BKDR_hash("Key3",4) % 64);
	printf("[Key4] = %d\n", amx_BKDR_hash("Key4",4) % 64);
	printf("[Key5] = %d\n", amx_BKDR_hash("Key5",4) % 64);
	printf("[Key6] = %d\n", amx_BKDR_hash("Key6",4) % 64);
	printf("[Key7] = %d\n", amx_BKDR_hash("Key7",4) % 64);
	printf("[Key8] = %d\n", amx_BKDR_hash("Key8",4) % 64);
	printf("[Key9] = %d\n", amx_BKDR_hash("Key9",4) % 64);
	printf("[Key11] = %d\n", amx_BKDR_hash("Key11",5) % 64);
	printf("[Key21] = %d\n", amx_BKDR_hash("Key21",5) % 64);
	printf("[Key31] = %d\n", amx_BKDR_hash("Key31",5) % 64);
	printf("[Key41] = %d\n", amx_BKDR_hash("Key41",5) % 64);
	printf("[Key51] = %d\n", amx_BKDR_hash("Key51",5) % 64);
	printf("[Key61] = %d\n", amx_BKDR_hash("Key61",5) % 64);
	printf("[Key71] = %d\n", amx_BKDR_hash("Key71",5) % 64);
	printf("[Key81] = %d\n", amx_BKDR_hash("Key81",5) % 64);
	printf("[Key91] = %d\n", amx_BKDR_hash("Key91",5) % 64);
	printf("[Key110] = %d\n", amx_BKDR_hash("Key110",6) % 64);
	printf("[Key210] = %d\n", amx_BKDR_hash("Key210",6) % 64);
	printf("[Key310] = %d\n", amx_BKDR_hash("Key310",6) % 64);
	printf("[Key410] = %d\n", amx_BKDR_hash("Key410",6) % 64);
	printf("[Key510] = %d\n", amx_BKDR_hash("Key510",6) % 64);
	printf("[Key610] = %d\n", amx_BKDR_hash("Key610",6) % 64);
	printf("[Key710] = %d\n", amx_BKDR_hash("Key710",6) % 64);
	printf("[Key810] = %d\n", amx_BKDR_hash("Key810",6) % 64);
	printf("[Key910] = %d\n", amx_BKDR_hash("Key910",6) % 64);
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

	tc = tcase_create ("amx_hash_distribution");
	tcase_add_test (tc, amx_hash_BKDR_hashes_check);
	suite_add_tcase (s, tc);

	return s;
}
