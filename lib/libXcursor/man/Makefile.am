libmandir = $(LIB_MAN_DIR)

libman_PRE =				\
	Xcursor.man			\
	$(Xcursor_shadows:=.man)

Xcursor_shadows =			\
	XcursorAnimateCreate		\
	XcursorAnimateDestroy		\
	XcursorAnimateNext		\
	XcursorCommentCreate		\
	XcursorCommentDestroy		\
	XcursorCommentsCreate		\
	XcursorCommentsDestroy		\
	XcursorCursorsCreate		\
	XcursorCursorsDestroy		\
	XcursorFileLoad			\
	XcursorFileLoadAllImages	\
	XcursorFileLoadImage		\
	XcursorFileLoadImages		\
	XcursorFileSave			\
	XcursorFileSaveImages		\
	XcursorFilenameLoad		\
	XcursorFilenameLoadAllImages	\
	XcursorFilenameLoadCursor	\
	XcursorFilenameLoadCursors	\
	XcursorFilenameLoadImage	\
	XcursorFilenameLoadImages	\
	XcursorFilenameSave		\
	XcursorFilenameSaveImages	\
	XcursorGetDefaultSize		\
	XcursorGetTheme			\
	XcursorGetThemeCore		\
	XcursorImageCreate		\
	XcursorImageDestroy		\
	XcursorImageHash		\
	XcursorImageLoadCursor		\
	XcursorImagesCreate		\
	XcursorImagesDestroy		\
	XcursorImagesLoadCursor		\
	XcursorImagesLoadCursors	\
	XcursorImagesSetName		\
	XcursorLibraryLoadCursor	\
	XcursorLibraryLoadCursors	\
	XcursorLibraryLoadImage		\
	XcursorLibraryLoadImages	\
	XcursorLibraryPath		\
	XcursorLibraryShape		\
	XcursorNoticeCreateBitmap	\
	XcursorNoticePutBitmap		\
	XcursorSetDefaultSize		\
	XcursorSetTheme			\
	XcursorSetThemeCore		\
	XcursorShapeLoadCursor		\
	XcursorShapeLoadCursors		\
	XcursorShapeLoadImage		\
	XcursorShapeLoadImages		\
	XcursorSupportsARGB		\
	XcursorSupportsAnim		\
	XcursorTryShapeBitmapCursor	\
	XcursorTryShapeCursor		\
	XcursorXcFileLoad		\
	XcursorXcFileLoadAllImages	\
	XcursorXcFileLoadImage		\
	XcursorXcFileLoadImages		\
	XcursorXcFileSave

libman_DATA = $(libman_PRE:man=@LIB_MAN_SUFFIX@)

EXTRA_DIST = $(libman_PRE)
CLEANFILES = $(libman_DATA)

SUFFIXES = .$(LIB_MAN_SUFFIX) .man

# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
# 's/:/, /g' will add comma and a space to help path formatting
MAN_SUBSTS += -e 's|__XCURSORPATH__|$(XCURSORPATH)|g' \
	      -e '\|$(XCURSORPATH)| s/:/, /g'

.man.$(LIB_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
