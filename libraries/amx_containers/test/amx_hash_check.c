#include <stdlib.h>
#include <stdio.h>

#include "amx_containers_check.h"
#include <amx_containers/amx_hash.h>

#include "mock_malloc.h"

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


Suite *amx_hash_suite(void)
{
	Suite *s = suite_create ("amx_hash");
	TCase *tc = NULL;

	tc = tcase_create ("amx_hash_generation");
	tcase_add_test (tc, amx_hash_generation_check);
	suite_add_tcase (s, tc);

	return s;
}
