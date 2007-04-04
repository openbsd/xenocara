# $OpenBSD: Makefile,v 1.5 2007/04/04 22:33:46 david Exp $

.include <bsd.own.mk>
USE_GCC3?=No

LIB=	GLU

DIST=	${.CURDIR}/../../dist/Mesa
GLU=	${DIST}/src/glu/sgi

# libutil
SRCS= \
	error.c \
	glue.c \
	mipmap.c \
	project.c \
	quad.c \
	registry.c \

# libtess
SRCS+= \
	dict.c \
	geom.c \
	memalloc.c \
	mesh.c \
	normal.c \
	priorityq.c \
	render.c \
	sweep.c \
	tess.c \
	tessmono.c

# libnurbs/interface 
SRCS+= \
	bezierEval.cc \
	bezierPatch.cc \
	bezierPatchMesh.cc \
	glcurveval.cc \
	glinterface.cc \
	glrenderer.cc \
	glsurfeval.cc \
	incurveeval.cc \
	insurfeval.cc \

# libnurbs/internals
SRCS+= \
	arc.cc \
	arcsorter.cc \
	arctess.cc \
	backend.cc \
	basiccrveval.cc \
	basicsurfeval.cc \
	bin.cc \
	bufpool.cc \
	cachingeval.cc \
	ccw.cc \
	coveandtiler.cc \
	curve.cc \
	curvelist.cc \
	curvesub.cc \
	dataTransform.cc \
	displaylist.cc \
	flist.cc \
	flistsorter.cc \
	hull.cc \
	intersect.cc \
	knotvector.cc \
	mapdesc.cc \
	mapdescv.cc \
	maplist.cc \
	mesher.cc \
	monoTriangulationBackend.cc \
	monotonizer.cc \
	mycode.cc \
	nurbsinterfac.cc \
	nurbstess.cc \
	patch.cc \
	patchlist.cc \
	quilt.cc \
	reader.cc \
	renderhints.cc \
	slicer.cc \
	sorter.cc \
	splitarcs.cc \
	subdivider.cc \
	tobezier.cc \
	trimline.cc \
	trimregion.cc \
	trimvertpool.cc \
	uarray.cc \
	varray.cc

# libnurbs/nurbtess
SRCS+= \
	directedLine.cc \
	gridWrap.cc \
	monoChain.cc \
	monoPolyPart.cc \
	monoTriangulation.cc \
	partitionX.cc \
	partitionY.cc \
	polyDBG.cc \
	polyUtil.cc \
	primitiveStream.cc \
	quicksort.cc \
	rectBlock.cc \
	sampleComp.cc \
	sampleCompBot.cc \
	sampleCompRight.cc \
	sampleCompTop.cc \
	sampleMonoPoly.cc \
	sampledLine.cc \
	searchTree.cc

CPPFLAGS+=	-I${DIST}/include \
		-I${GLU}/include \
		-I${GLU}/internals \
		-I${GLU}/libnurbs/internals \
		-I${GLU}/libnurbs/interface \
		-I${GLU}/libnurbs/nurbtess \
		-DLIBRARYBUILD -D_EXTENSIONS_
.if ${USE_GCC3:L} == "yes"
LDADD+=	-lsupc++
.endif

NOPROFILE=

obj: _xenocara_obj

includes:
	@cmp -s ${DIST}/include/GL/glu.h \
		${DESTDIR}${X11BASE}/include/GL/glu.h \
	|| ${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		${DIST}/include/GL/glu.h ${DESTDIR}${X11BASE}/include/GL 

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${GLU}/libutil
.PATH: ${GLU}/libtess
.PATH: ${GLU}/libnurbs/interface
.PATH: ${GLU}/libnurbs/internals
.PATH: ${GLU}/libnurbs/nurbtess
