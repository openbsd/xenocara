
appmandir = $(APP_MAN_DIR)
appman_PRE = startx.man  xinit.man
appman_DATA = $(appman_PRE:man=$(APP_MAN_SUFFIX))

EXTRA_DIST = $(appman_PRE)
CLEANFILES = $(appman_DATA)
SUFFIXES = .$(APP_MAN_SUFFIX) .man

MAN_SUBSTS+=	-e 's|__XSERVERNAME__|$(XSERVERNAME)|g' \
		-e 's|__XCONFIGFILE__|$(XCONFIGFILE)|g' \
		-e 's|__XCONFIGFILEMAN__|$(XCONFIGFILEMAN)|g' \
		-e 's|__xinitdir__|$(XINITDIR)|g' \
		-e 's|__bindir__|$(bindir)|g' \
		-e 's|__libdir__|$(libdir)|g' \
		-e 's|__SCOMAN__|$(SCOMAN)|g' \
		-e 's|__configdir__|$(XINITDIR)|g'


# String replacements in MAN_SUBSTS now come from xorg-macros.m4 via configure
.man.$(APP_MAN_SUFFIX):
	$(AM_V_GEN)$(SED) $(MAN_SUBSTS) < $< > $@
