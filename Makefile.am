AM_CFLAGS = $(all_includes)

SUBDIRS = src

EXTRA_DIST = README.org doc autogen.sh

# Create documentation
README.md : README.org
	emacs README.org --batch -f org-md-export-to-markdown

doc : src doxygen.dox
	doxygen ${top_srcdir}/doxygen.dox

dist_doc_DATA = README.md doxygen.dox

clean-local:
	rm -rf doc

