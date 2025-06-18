bin_PROGRAMS = afstest

afstest_CFLAGS = \
    -DUNIT_TESTS_ENABLED \
    -I$(top_srcdir)/inc \
    -I$(top_srcdir)/thirdparty/minixml/inc \
    -I$(LIBUNBOXING_DIR)/inc \
    -I$(LIBUNBOXING_DIR)/thirdparty/glib \
    -I$(LIBUNBOXING_DIR)/tests/testutils/inc

afstest_LDADD = \
    $(top_srcdir)/src/libafs.a \
    $(LIBUNBOXING_DIR)/src/libunboxing.a \
    -lcheck \
    -lsubunit \
    -lm

afstest_SOURCES = \
    main.c \
    controldatatests.c \
    controlframetests.c \
    framerangestests.c \
    framerangetests.c \
    previewtests.c \
    sha1tests.c \
    sha256tests.c \
    tocdatafilemetadatasourcetests.c \
    tocdatafilemetadatatests.c \
    tocdatareelstests.c \
    tocdatareeltests.c \
    tocdatatests.c \
    tocfilepreviewpagetests.c \
    tocfilepreviewtests.c \
    tocmetadatasourcetests.c \
    tocmetadatatests.c \
    tocpreviewlayoutdefinitionstests.c \
    tocpreviewlayoutdefinitiontests.c \
    tocpreviewsectionstests.c \
    tocpreviewsectiontests.c \
    $(LIBUNBOXING_DIR)/tests/testutils/src/boxing_config.c
