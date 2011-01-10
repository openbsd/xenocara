#	$OpenBSD: Makefile.inc,v 1.1 2011/01/10 14:42:48 shadchin Exp $

TOP=		../../../..

BINDIR=		${XKB_DIR}/${XKB_SUBDIR}/../bin
SRCDIR=		${XKB_DIR}/${XKB_SUBDIR}

LST_FILES=	layoutsMapping.lst variantsMapping.lst

.include "../Makefile.inc"
