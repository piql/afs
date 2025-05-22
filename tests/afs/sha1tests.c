#pragma GCC diagnostic ignored "-Wunused-variable"

#include "unittests.h"
#include "sha1.h"
#include "boxing/utils.h"
#include "boxing/string.h"
#include <stdio.h>
#include <string.h>

// Test function afs_sha1_test to test sha1 core
BOXING_START_TEST(afs_sha1_test_test1)
{
    const int test_result = afs_sha1_test();

    if (test_result != 0)
    {
        printf("SHA1 test result = %d\n", test_result);
    }

    BOXING_ASSERT(test_result == 0);
}
END_TEST

// Test function afs_sha1_sum1 with input file name equal to NULL
BOXING_START_TEST(afs_sha1_sum1_test1)
{
    const char* file_name = NULL;

    char* hash;
    hash = afs_sha1_sum1(file_name);

    BOXING_ASSERT(hash == NULL);
}
END_TEST


// Test function afs_sha1_sum1 with normal input file
BOXING_START_TEST(afs_sha1_sum1_test2)
{
    const char* file_name = "test_file.txt";
    FILE *test_file = fopen(file_name, "wb");
    if (test_file != NULL)
    {
        const char* test_text = "Here is a minimal example C implementation to hash a C-string.";
        fprintf(test_file, "%s\n", test_text);
    }
    BOXING_ASSERT(fclose(test_file) == 0);

    char* hash1 = "6600abf17befdd9de7fa1d4503c7171abed2bdd2";
    char* hash2;
    hash2 = afs_sha1_sum1(file_name);
    remove(file_name);

    BOXING_ASSERT(hash2 != NULL);
    BOXING_ASSERT(strcmp(hash1, hash2) == 0);

    free(hash2);
}
END_TEST


// Test function afs_sha1_sum1 when input file not exist
BOXING_START_TEST(afs_sha1_sum1_test3)
{
    const char* file_name = "test_file.txt";

    char* hash;
    hash = afs_sha1_sum1(file_name);

    BOXING_ASSERT(hash == NULL);
}
END_TEST

// Test function afs_sha1_sum1 when input file is >4GB
BOXING_START_TEST(afs_sha1_sum1_test4)
{
#if defined ( D_OS_WIN32 )
    BOXING_ASSERT(sizeof(__int64) == 8);
#else
    BOXING_ASSERT(sizeof(off_t) == 8);
#endif
    
    const char* file_name = "test_file.4gb";
    FILE *test_file = fopen(file_name, "wb");
    BOXING_ASSERT( test_file != NULL );
    boxing_int64 file_size = 1024 + ( 4ULL * 1024 * 1024 * 1024 );
    boxing_int64 bytes_left = file_size;
    char buffer[1024];
    memset( buffer, 0, sizeof(buffer) );
    while ( bytes_left )
    {
        BOXING_ASSERT( 1 == fwrite( buffer, sizeof(buffer), 1, test_file ) );
        bytes_left -= 1024;
    }

    BOXING_ASSERT(fclose(test_file) == 0);

    char* hash1 = "32667297e8ab1c3ec0ca4aa1554c5bfd277fd2fb";
    char* hash2;
    hash2 = afs_sha1_sum1(file_name);
    remove(file_name);

    BOXING_ASSERT(hash2 != NULL);
    BOXING_ASSERT(strcmp(hash1, hash2) == 0);

    free(hash2);
}
END_TEST


// Test function afs_sha1_sum2 with input file name equal to NULL
BOXING_START_TEST(afs_sha1_sum2_test1)
{
    const char* file_name = NULL;

    char* hash;
    hash = afs_sha1_sum2(file_name, 0, 0);

    BOXING_ASSERT(hash == NULL);
}
END_TEST


// Test function afs_sha1_sum2 when input file not exist
BOXING_START_TEST(afs_sha1_sum2_test2)
{
    const char* file_name = "test_file.txt";

    char* hash;
    hash = afs_sha1_sum2(file_name, 0, 0);

    BOXING_ASSERT(hash == NULL);
}
END_TEST


// Test function afs_sha1_sum2 with normal input file
BOXING_START_TEST(afs_sha1_sum2_test3)
{
    const char* file_name = "test_file.txt";
    FILE *test_file = fopen(file_name, "wb");
    if (test_file != NULL)
    {
        const char* test_text = "Here is a minimal example C implementation to hash a C-string.";
        fprintf(test_file, "%s\n", test_text);
    }
    BOXING_ASSERT(fclose(test_file) == 0);

    char* hash1 = "6600abf17befdd9de7fa1d4503c7171abed2bdd2";
    char* hash2 = afs_sha1_sum2(file_name, 0, -1);
    remove(file_name);

    BOXING_ASSERT(hash2 != NULL);
    BOXING_ASSERT(strcmp(hash1, hash2) == 0);

    free(hash2);
}
END_TEST


// Test function afs_sha1_sum2 with input size larger than input file size
BOXING_START_TEST(afs_sha1_sum2_test4)
{
    const char* file_name = "test_file.txt";
    FILE *test_file = fopen(file_name, "wb");
    if (test_file != NULL)
    {
        const char* test_text = "Here is a minimal example C implementation to hash a C-string.";
        fprintf(test_file, "%s\n", test_text);
    }
    BOXING_ASSERT(fclose(test_file) == 0);

    char* hash2 = afs_sha1_sum2(file_name, 0, 100000);
    remove(file_name);

    BOXING_ASSERT(hash2 == NULL);
}
END_TEST


// Test function afs_sha1_sum2 with start position outside the input file
BOXING_START_TEST(afs_sha1_sum2_test5)
{
    const char* file_name = "test_file.txt";
    FILE *test_file = fopen(file_name, "wb");
    if (test_file != NULL)
    {
        const char* test_text = "Here is a minimal example C implementation to hash a C-string.";
        fprintf(test_file, "%s\n", test_text);
    }
    BOXING_ASSERT(fclose(test_file) == 0);

    char* hash2 = afs_sha1_sum2(file_name, 1000000, 0);
    remove(file_name);

    BOXING_ASSERT(hash2 == NULL);
}
END_TEST


Suite * sha1_tests(void)
{
    TCase * tc_sha1_functions_tests = tcase_create("sha1_functions_tests");
    tcase_set_timeout( tc_sha1_functions_tests, 60*60 ); // 1 hour timeout

    // Test function afs_sha1_test
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_test_test1);
    // Test function afs_sha1_sum1
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum1_test1);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum1_test2);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum1_test3);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum1_test4);
    // Test function afs_sha1_sum2
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum2_test1);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum2_test2);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum2_test3);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum2_test4);
    tcase_add_test(tc_sha1_functions_tests, afs_sha1_sum2_test5);

    Suite * s = suite_create("sha1_test_util");
    suite_add_tcase(s, tc_sha1_functions_tests);

    return s;
}

