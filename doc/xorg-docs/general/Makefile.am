
SUBDIRS = fonts input graphics platforms

if ENABLE_DOCS

# Main DocBook/XML files (DOCTYPE book)
docbook =  			\
	License.xml		\
	README.xml		\
	ReleaseNotes.xml	\
	Versions.xml

# The location where the DocBook/XML files and their generated formats are installed
shelfdir = $(docdir)

# Generate DocBook/XML output formats with or without stylesheets
include $(top_srcdir)/docbook.am

endif ENABLE_DOCS
