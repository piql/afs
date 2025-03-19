AM_CFLAGS = $(all_includes)

SUBDIRS = src

afsincludedir = $(includedir)/afs
afsinclude_HEADERS = \
    inc/administrativemetadata.h \
    inc/controldata.h \
    inc/controlframeboxingformat.h \
    inc/framerange.h \
    inc/frameranges.h \
    inc/sha1.h \
    inc/sha1hash.h \
    inc/sha256.h \
    inc/sha256hash.h \
    inc/technicalmetadata.h \
    inc/tocdata_c.h \
    inc/tocdatafilemetadata_c.h \
    inc/tocdatafilemetadatasource_c.h \
    inc/tocdatareel_c.h \
    inc/tocdatareels.h \
    inc/tocfile.h \
    inc/tocfilepreview.h \
    inc/tocfilepreviewpage.h \
    inc/tocfiles.h \
    inc/tocmetadata_c.h \
    inc/tocmetadatasource_c.h \
    inc/xmlutils.h \
    inc/zeroreferenceframe.h


afscontrolframeincludedir = $(includedir)/afs/controlframe
afscontrolframeinclude_HEADERS = \
    inc/controlframe/boxingformat.h

afsplatformincludedir = $(includedir)/afs/platform
afsplatforminclude_HEADERS = \
    inc/platform/io.h

afscfincludedir = $(includedir)/afs/controlframe
afscfinclude_HEADERS = \
    inc/controlframe/boxingformat.h

afstocincludedir = $(includedir)/afs/toc
afstocinclude_HEADERS = \
    inc/toc/previewimageutils.h \
    inc/toc/previewlayoutdefinition.h \
    inc/toc/previewlayoutdefinitions.h \
    inc/toc/previewsection.h \
    inc/toc/previewsections.h

include_HEADERS = \
	thirdparty/minixml/inc/mxml.h

EXTRA_DIST = README.org doc autogen.sh

# Create documentation
README.md : README.org
	emacs README.org --batch -f org-md-export-to-markdown

doc : src doxygen.dox
	doxygen ${top_srcdir}/doxygen.dox

dist_doc_DATA = README.md doxygen.dox

clean-local:
	rm -rf doc

