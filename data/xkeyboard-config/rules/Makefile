#	$OpenBSD: Makefile,v 1.2 2009/06/06 19:43:22 matthieu Exp $

SUBDIR=	compat

XKB_SUBDIR=	rules

DATA=	xkb.dtd README

base_parts = base.hdr.part base.lists.part \
	compat/base.lists.part \
	HDR base.m_k.part \
	HDR base.l1_k.part \
	HDR base.l_k.part \
	HDR base.o_k.part \
	HDR base.ml_g.part \
	HDR base.m_g.part \
	HDR compat/base.mlv_s.part base.mlv_s.part \
	HDR compat/base.ml_s.part base.ml_s.part  \
	HDR compat/base.ml1_s.part base.ml1_s.part \
	HDR compat/base.ml1v1_s.part \
	HDR compat/base.l2_s.part base.l2_s.part  \
	HDR compat/base.l3_s.part base.l3_s.part  \
	HDR compat/base.l4_s.part base.l4_s.part  \
	HDR compat/base.l2v2_s.part \
	HDR compat/base.l3v3_s.part \
	HDR compat/base.l4v4_s.part \
	HDR base.m_s.part \
	HDR base.ml_c.part \
	HDR base.ml1_c.part \
	HDR base.m_t.part \
	HDR base.lo_s.part \
	HDR base.l1o_s.part \
	HDR base.l2o_s.part \
	HDR base.l3o_s.part \
	HDR base.l4o_s.part \
	HDR compat/base.o_s.part base.o_s.part \
	HDR base.o_c.part \
	HDR base.o_t.part

files= base base.lst base.xml

srcdir=	${XKB_DIR}/rules

all:	_SUBDIRUSE ${files}

depend:	_SUBDIRUSE

base: $(base_parts)
	HDR="$(srcdir)/HDR" $(srcdir)/merge.sh $@ $(base_parts)

base.lst: base.xml
	perl $(srcdir)/xml2lst.pl < base.xml > $@

base.xml: base.xml.in
	sed -e 's,<_,<,g' -e 's,</_,</,g' ${XKB_DIR}/${XKB_SUBDIR}/base.xml.in > $@
install-files: ${files}
	@for i in ${files}; do  \
		echo "installing $$i"; \
		${INSTALL_DATA} $$i ${XKB_BASE}/${XKB_SUBDIR}; \
	done

clean: _SUBDIRUSE
	rm -f base base.lst base.xml 

.include <bsd.xorg.mk>
.include <bsd.subdir.mk>

.PATH:	${XKB_DIR}/rules
.SUFFIXES: .in .lst .xml
