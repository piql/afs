bin_PROGRAMS = assemblytool

assemblytool_CFLAGS = \
    -I$(top_srcdir)/inc \
    -I$(top_srcdir)/thirdparty/minixml/inc \
    -I$(LIBUNBOXING_DIR)/inc \
    -I$(LIBUNBOXING_DIR)/thirdparty/glib \
    -I$(LIBUNBOXING_DIR)/tests/testutils/inc

assemblytool_LDADD = \
    $(top_srcdir)/src/libafs.a \
    $(LIBUNBOXING_DIR)/src/libunboxing.a \
    -lm

assemblytool_SOURCES = \
    main.c \
    unboxingtool.c \
    $(LIBUNBOXING_DIR)/tests/testutils/src/boxing_config.c
