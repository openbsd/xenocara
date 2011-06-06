#	$OpenBSD: Makefile.inc,v 1.2 2011/06/06 19:45:44 matthieu Exp $

XKC_TOP=		../../../..

BINDIR=		${XKB_DIR}/${XKB_SUBDIR}/../bin
SRCDIR=		${XKB_DIR}/${XKB_SUBDIR}

LST_FILES=	layoutsMapping.lst variantsMapping.lst

.include "../Makefile.inc"
