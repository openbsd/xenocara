#	$OpenBSD: Makefile,v 1.1 2011/01/10 14:42:48 shadchin Exp $

XKB_SUBDIR=	rules/extras

EXTRAS_PARTS=	base.ml2_s.part base.ml3_s.part base.ml4_s.part \
		base.ml2v2_s.part base.ml3v3_s.part base.ml4v4_s.part \
		base.ml_s.part base.ml1_s.part base.mlv_s.part \
		base.ml1v1_s.part base.ml_s.part

depend all:	${EXTRAS_PARTS}

base.ml2_s.part:	${LST_FILES}
	sh ${BINDIR}/mln_s.sh 2 ${SRCDIR}

base.ml3_s.part:	${LST_FILES}
	sh ${BINDIR}/mln_s.sh 3 ${SRCDIR}

base.ml4_s.part:	${LST_FILES}
	sh ${BINDIR}/mln_s.sh 4 ${SRCDIR}

base.ml2v2_s.part:	${LST_FILES}
	sh ${BINDIR}/mlnvn_s.sh 2 ${SRCDIR}

base.ml3v3_s.part:	${LST_FILES}
	sh ${BINDIR}/mlnvn_s.sh 3 ${SRCDIR}

base.ml4v4_s.part:	${LST_FILES}
	sh ${BINDIR}/mlnvn_s.sh 4 ${SRCDIR}

base.ml_s.part:	${LST_FILES}
	sh ${BINDIR}/ml_s.sh ${SRCDIR}

base.ml1_s.part:	${LST_FILES}
	sh ${BINDIR}/ml1_s.sh ${SRCDIR}

base.mlv_s.part:	${LST_FILES}
	sh ${BINDIR}/mlv_s.sh ${SRCDIR}

base.ml1v1_s.part:	${LST_FILES}
	sh ${BINDIR}/ml1v1_s.sh ${SRCDIR}

clean:
	rm -f ${EXTRAS_PARTS}

.include <bsd.xorg.mk>
