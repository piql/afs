#include "unittests.h"
#include "boxing/utils.h"

#include <stdarg.h>
#include <stdio.h>

extern Suite * preview_tests();
extern Suite * tocpreviewsection_tests();
extern Suite * tocpreviewsections_tests();
extern Suite * tocpreviewlayoutdefinition_tests();
extern Suite * tocpreviewlayoutdefinitions_tests();
extern Suite * tocdatafilemetadatasource_tests();
extern Suite * tocdatafilemetadata_tests();
extern Suite * sha1_tests();
extern Suite * sha256_tests();
extern Suite * controlframe_tests();
extern Suite * controldata_tests();
extern Suite * tocdatafile_tests();
extern Suite * tocdatafiles_tests();
extern Suite * tocfilepreview_tests();
extern Suite * tocfilepreviewpage_tests();
extern Suite * tocmetadata_tests();
extern Suite * tocmetadatasource_tests();
extern Suite * zeroreferencemark_tests();
extern Suite * framerange_tests();
extern Suite * frameranges_tests();
extern Suite * tocdatareel_tests();
extern Suite * tocdatareels_tests();
extern Suite * tocdata_tests();
typedef Suite* (*suiteFp)();

Suite * make_master_suite( void )
{
    return suite_create( "afslib" );
}

int main(int argc, char *argv[])
{
    BOXING_UNUSED_PARAMETER( argv );
    BOXING_UNUSED_PARAMETER( argc );

    int number_failed;
    SRunner *sr;

    sr = srunner_create(make_master_suite());
    srunner_set_xml( sr, "afs.xml" );

    suiteFp suites[] =
    {
        controldata_tests,
        controlframe_tests,
        framerange_tests,
        frameranges_tests,
        preview_tests,
        sha1_tests,
        sha256_tests,
        tocdata_tests,
        tocdatafilemetadata_tests,
        tocdatafilemetadatasource_tests,
        tocdatareel_tests,
        tocdatareels_tests,
        tocfilepreview_tests,
        tocfilepreviewpage_tests,
        tocmetadata_tests,
        tocmetadatasource_tests,
        tocpreviewlayoutdefinition_tests,
        tocpreviewlayoutdefinitions_tests,
        tocpreviewsection_tests,
        tocpreviewsections_tests,
    };
    
    // Add all test suites here
    for (size_t s = 0; s < sizeof(suites)/sizeof(suites[0]); s++)
    {
        srunner_add_suite(sr, suites[s]());
    }
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ?
                EXIT_SUCCESS :
                EXIT_FAILURE;
}

void boxing_log( int log_level, const char * string )
{
    printf( "%d : %s\n", log_level, string );
}

void boxing_log_args( int log_level, const char * format, ... )
{
    va_list args;
    va_start(args, format);

    printf( "%d : ", log_level );
    vprintf( format, args );
    printf( "\n" );

    va_end(args);
}
