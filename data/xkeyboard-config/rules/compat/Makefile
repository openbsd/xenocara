#	$OpenBSD: Makefile,v 1.2 2009/06/06 19:42:19 matthieu Exp $

XKB_SUBDIR=	rules/compat
TOP=		../../../..

transform_files=layoutRename.lst variantRename.lst

transform_scripts= \
	ln_s.sh \
	lnv_s.sh \
	ml_s.sh \
	mlv_s.sh \
	ml1_s.sh \
	ml1v_s.sh \
	ml1v1_s.sh

dist_parts=base.o_s.part

scripts= \
	base.l2_s.part \
	base.l3_s.part \
	base.l4_s.part \
	base.l2v2_s.part \
	base.l3v3_s.part \
	base.l4v4_s.part \
	base.ml_s.part \
	base.ml1_s.part \
	base.mlv_s.part \
	base.ml1v1_s.part \
	base.ml_s.part

depend all:	${scripts}

srcdir=	${XKB_DIR}/${XKB_SUBDIR}

base.l2_s.part: ln_s.sh $(transform_files)
	sh  ${srcdir}/ln_s.sh 2

base.l3_s.part: ln_s.sh $(transform_files)
	sh ${srcdir}/ln_s.sh 3

base.l4_s.part: ln_s.sh $(transform_files)
	sh ${srcdir}/ln_s.sh 4

base.l2v2_s.part: lnv_s.sh $(transform_files)
	sh ${srcdir}/lnv_s.sh 2

base.l3v3_s.part: lnv_s.sh $(transform_files)
	sh ${srcdir}/lnv_s.sh 3

base.l4v4_s.part: lnv_s.sh $(transform_files)
	sh ${srcdir}/lnv_s.sh 4

base.ml_s.part: ml_s.sh $(transform_files)
	sh ${srcdir}/ml_s.sh

base.ml1_s.part: ml1_s.sh $(transform_files)
	sh ${srcdir}/ml1_s.sh

base.mlv_s.part: mlv_s.sh $(transform_files)
	sh ${srcdir}/mlv_s.sh

base.ml1v1_s.part: ml1v1_s.sh $(transform_files)
	sh ${srcdir}/ml1v1_s.sh

clean:
	rm -f ${scripts}

.include "../../Makefile.inc"
.include <bsd.xorg.mk>

.SUFFIXES: .sh
.PATH: ${XSRCDIR}/dist/xkeyboard-config/rules/compat
