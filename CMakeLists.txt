cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(afs C)

add_subdirectory(unboxing EXCLUDE_FROM_ALL)

if(WIN32)
    set(WARNINGS /MP)
elseif(APPLE)
    set(WARNINGS -Wall -Wextra -Wpedantic -pedantic-errors -Wformat-overflow -Wstrict-overflow=2 -Werror)
else()
    set(WARNINGS -Wall -Wextra -Wpedantic -pedantic-errors -Wformat-overflow=1 -Wstrict-overflow=2 -Werror)
endif()

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
target_compile_options(minixml PRIVATE ${WARNINGS})
target_include_directories(minixml PUBLIC thirdparty/minixml/inc)
if(WIN32)
    target_compile_definitions(minixml PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(minixml PRIVATE -Wno-use-after-free)
endif()

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
target_compile_options(afs PRIVATE ${WARNINGS})
target_include_directories(afs PUBLIC inc)
target_link_libraries(afs unboxing minixml)
if(WIN32)
    target_compile_definitions(afs PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

set_target_properties(afs minixml PROPERTIES
    C_EXTENSIONS OFF
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

if(PROJECT_IS_TOP_LEVEL)
    set(BUILD_TESTING ON)
endif()

if(BUILD_TESTING)
    enable_testing()

    if(NOT WIN32 AND NOT APPLE)
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
            tests/afs/tocfilestests.c
            tests/afs/tocfiletests.c
            tests/afs/tocmetadatasourcetests.c
            tests/afs/tocmetadatatests.c
            tests/afs/tocpreviewlayoutdefinitionstests.c
            tests/afs/tocpreviewlayoutdefinitiontests.c
            tests/afs/tocpreviewsectionstests.c
            tests/afs/tocpreviewsectiontests.c
            tests/afs/zeroreferenceframetests.c
        )
        target_compile_options(afstest PRIVATE ${WARNINGS})
        target_link_libraries(afstest afs testutils check subunit)
        add_test(NAME afstest COMMAND afstest)
        set_target_properties(afstest PROPERTIES C_EXTENSIONS OFF C_STANDARD 99 C_STANDARD_REQUIRED ON)

        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/doc/html/index.html"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMAND doxygen doxygen.dox
            DEPENDS doxygen.dox
        )
        add_custom_target(doxygen ALL DEPENDS doc/html/index.html)
    endif()

    add_executable(assemblytool
        tests/assemblytool/main.c
        tests/assemblytool/unboxingtool.c
    )
    target_compile_options(assemblytool PRIVATE ${WARNINGS})
    target_include_directories(assemblytool PRIVATE inc)
    target_link_libraries(assemblytool afs testutils)
    if(WIN32)
        target_compile_definitions(assemblytool PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()

    set_target_properties(assemblytool PROPERTIES C_EXTENSIONS OFF C_STANDARD 99 C_STANDARD_REQUIRED ON)
endif()
