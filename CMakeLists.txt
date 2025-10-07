cmake_minimum_required(VERSION 3.10)

project(afs)

add_subdirectory(thirdparty/unboxing EXCLUDE_FROM_ALL)

add_library(minixml
    thirdparty/minixml/src/mxml-attr.c
    thirdparty/minixml/src/mxml-entity.c
    thirdparty/minixml/src/mxml-file.c
    thirdparty/minixml/src/mxml-get.c
    thirdparty/minixml/src/mxml-index.c
    thirdparty/minixml/src/mxml-node.c
    thirdparty/minixml/src/mxml-private.c
    thirdparty/minixml/src/mxml-search.c
    thirdparty/minixml/src/mxml-set.c
    thirdparty/minixml/src/mxml-string.c
)

target_include_directories(minixml PUBLIC thirdparty/minixml/inc)

add_library(afs
    src/administrativemetadata.c
    src/afs.c
    src/controldata.c
    src/controlframe/boxingformat.c
    src/controlframeboxingformat.c
    src/framerange.c
    src/frameranges.c
    src/sha1.c
    src/sha1hash.c
    src/sha256.c
    src/sha256hash.c
    src/technicalmetadata.c
    src/toc/previewimageutils.c
    src/toc/previewlayoutdefinition.c
    src/toc/previewlayoutdefinitions.c
    src/toc/previewsection.c
    src/toc/previewsections.c
    src/tocdata.c
    src/tocdatafilemetadata.c
    src/tocdatafilemetadatasource.c
    src/tocdatareel.c
    src/tocdatareels.c
    src/tocfile.c
    src/tocfilepreview.c
    src/tocfilepreviewpage.c
    src/tocfiles.c
    src/tocmetadata.c
    src/tocmetadatasource.c
    src/xmlutils.c
    src/zeroreferenceframe.c
)

target_include_directories(afs PUBLIC inc)

target_link_libraries(afs unboxing minixml)

if(PROJECT_IS_TOP_LEVEL)
    set(BUILD_TESTING ON)
endif()

if(BUILD_TESTING)
    enable_testing()

    add_executable(afstest
        tests/afs/controldatatests.c
        tests/afs/controlframetests.c
        tests/afs/framerangestests.c
        tests/afs/framerangetests.c
        tests/afs/main.c
        tests/afs/previewtests.c
        tests/afs/sha1tests.c
        tests/afs/sha256tests.c
        tests/afs/tocdatafilemetadatasourcetests.c
        tests/afs/tocdatafilemetadatatests.c
        tests/afs/tocdatareelstests.c
        tests/afs/tocdatareeltests.c
        tests/afs/tocdatatests.c
        tests/afs/tocfilepreviewpagetests.c
        tests/afs/tocfilepreviewtests.c
        tests/afs/tocmetadatasourcetests.c
        tests/afs/tocmetadatatests.c
        tests/afs/tocpreviewlayoutdefinitionstests.c
        tests/afs/tocpreviewlayoutdefinitiontests.c
        tests/afs/tocpreviewsectionstests.c
        tests/afs/tocpreviewsectiontests.c
    )

    target_compile_definitions(afstest PRIVATE UNIT_TESTS_ENABLED)
    target_link_libraries(afstest afs testutils -lcheck -lsubunit)

    add_executable(assemblytool
        tests/assemblytool/main.c
        tests/assemblytool/unboxingtool.c
    )

    target_include_directories(assemblytool PRIVATE inc)

    target_link_libraries(assemblytool afs testutils)

    add_test(NAME afstest COMMAND afstest)

    add_custom_command(OUTPUT ../README.md WORKING_DIRECTORY .. COMMAND emacs README.org --batch -f org-md-export-to-markdown DEPENDS README.org)
    add_custom_target(readme ALL DEPENDS README.md)
    add_custom_command(OUTPUT ../doc/html/index.html WORKING_DIRECTORY .. COMMAND doxygen doxygen.dox DEPENDS doxygen.dox)
    add_custom_target(doxygen ALL DEPENDS doc/html/index.html)
endif()
