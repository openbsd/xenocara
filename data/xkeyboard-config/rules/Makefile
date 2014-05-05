#	$OpenBSD: Makefile,v 1.10 2014/05/05 18:05:33 shadchin Exp $

SUBDIR=		compat

XKB_SUBDIR=	rules

DATA=		xkb.dtd README

BASE_PARTS=	base.hdr.part base.lists.part \
		base.lists.base.part \
		compat/base.lists.part \
		HDR base.m_k.part \
		HDR base.l1_k.part \
		HDR base.l_k.part \
		HDR base.o_k.part \
		HDR base.ml_g.part \
		HDR base.m_g.part \
		HDR compat/base.mlv_s.part    base.mlv_s.part \
		HDR compat/base.ml_s.part     base.ml_s.part  \
		HDR compat/base.ml1_s.part    base.ml1_s.part \
		HDR compat/base.ml1v1_s.part \
		HDR compat/base.ml2_s.part    base.ml2_s.part \
		HDR compat/base.ml3_s.part    base.ml3_s.part \
		HDR compat/base.ml4_s.part    base.ml4_s.part \
		HDR compat/base.ml2v2_s.part \
		HDR compat/base.ml3v3_s.part \
		HDR compat/base.ml4v4_s.part \
		HDR base.m_s.part \
		HDR base.ml_s1.part \
		HDR compat/base.lv_c.part   \
		HDR compat/base.l1v1_c.part \
		HDR compat/base.l2v2_c.part \
		HDR compat/base.l3v3_c.part \
		HDR compat/base.l4v4_c.part \
		HDR base.ml_c.part \
		HDR base.ml1_c.part \
		HDR base.m_t.part \
		HDR base.lo_s.part \
		HDR base.l1o_s.part \
		HDR base.l2o_s.part \
		HDR base.l3o_s.part \
		HDR base.l4o_s.part \
		HDR compat/base.o_s.part     base.o_s.part \
		HDR base.o_c.part \
		HDR base.o_t.part

EVDEV_PARTS=	base.hdr.part base.lists.part \
		evdev.lists.part \
		compat/base.lists.part \
		HDR evdev.m_k.part \
		HDR base.l1_k.part \
		HDR base.l_k.part \
		HDR base.o_k.part \
		HDR base.ml_g.part \
		HDR base.m_g.part \
		HDR compat/base.mlv_s.part   base.mlv_s.part \
		HDR compat/base.ml_s.part    base.ml_s.part  \
		HDR compat/base.ml1_s.part   base.ml1_s.part \
		HDR compat/base.ml1v1_s.part \
		HDR compat/base.ml2_s.part    base.ml2_s.part  \
		HDR compat/base.ml3_s.part    base.ml3_s.part  \
		HDR compat/base.ml4_s.part    base.ml4_s.part  \
		HDR compat/base.ml2v2_s.part  \
		HDR compat/base.ml3v3_s.part  \
		HDR compat/base.ml4v4_s.part  \
		HDR evdev.m_s.part \
		HDR \
		HDR compat/base.lv_c.part   \
		HDR compat/base.l1v1_c.part \
		HDR compat/base.l2v2_c.part \
		HDR compat/base.l3v3_c.part \
		HDR compat/base.l4v4_c.part \
		HDR base.ml_c.part \
		HDR base.ml1_c.part \
		HDR base.m_t.part \
		HDR base.lo_s.part \
		HDR base.l1o_s.part \
		HDR base.l2o_s.part \
		HDR base.l3o_s.part \
		HDR base.l4o_s.part \
		HDR compat/base.o_s.part     base.o_s.part \
		HDR base.o_c.part \
		HDR base.o_t.part

BASE_RULES=		base base.lst base.xml base.extras.xml
EVDEV_RULES=		evdev evdev.lst evdev.xml evdev.extras.xml

all:	parts_done ${BASE_RULES} ${EVDEV_RULES} _SUBDIRUSE

parts_done:
	cd ${.CURDIR} && exec ${MAKE} depend
	touch $@

${BASE_PARTS}: parts_done

depend:	_SUBDIRUSE

base:	${BASE_PARTS}
	HDR="${XKB_DIR}/${XKB_SUBDIR}/HDR" ${SHELL} ${XKB_DIR}/${XKB_SUBDIR}/merge.sh $@ ${BASE_PARTS}

base.lst:	base.xml
	perl ${XKB_DIR}/${XKB_SUBDIR}/xml2lst.pl < base.xml > $@

base.xml:	base.xml.in
	sed -e 's,<_,<,g' -e 's,</_,</,g' ${XKB_DIR}/${XKB_SUBDIR}/base.xml.in > $@

base.extras.xml:	base.extras.xml.in
	sed -e 's,<_,<,g' -e 's,</_,</,g' ${XKB_DIR}/${XKB_SUBDIR}/base.extras.xml.in > $@

evdev:	${EVDEV_PARTS}
	HDR="${XKB_DIR}/${XKB_SUBDIR}/HDR" ${SHELL} ${XKB_DIR}/${XKB_SUBDIR}/merge.sh $@ ${EVDEV_PARTS}

evdev.lst:	evdev.xml
	perl ${XKB_DIR}/${XKB_SUBDIR}/xml2lst.pl < evdev.xml > $@

evdev.xml:	evdev.xml.in
	sed -e 's,<_,<,g' -e 's,</_,</,g' ${XKB_DIR}/${XKB_SUBDIR}/evdev.xml.in > $@

evdev.extras.xml:	evdev.extras.xml.in
	sed -e 's,<_,<,g' -e 's,</_,</,g' ${XKB_DIR}/${XKB_SUBDIR}/evdev.extras.xml.in > $@

install-files:	${BASE_RULES} ${EVDEV_RULES}
	test -z ${XKB_BASE}/${XKB_SUBDIR} || ${INSTALL_DIR} ${XKB_BASE}/${XKB_SUBDIR}
	@for f in ${BASE_RULES} ${EVDEV_RULES}; do  \
		echo ${INSTALL_DATA} $$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
		${INSTALL_DATA} $$f ${XKB_BASE}/${XKB_SUBDIR}/$$f; \
	done

clean:	_SUBDIRUSE
	rm -f ${BASE_RULES} ${EVDEV_RULES} parts_done

.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
