# $OpenBSD: Makefile,v 1.17 2012/08/17 15:16:44 mpi Exp $

.include <bsd.own.mk>
DEBUG?=

MESA_MAJOR=	7
MESA_MINOR=	11
MESA_TINY=	2

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

.if ${COMPILER_VERSION:L:Mgcc[34]*}
LDADD+=	-lsupc++
.endif

NOPROFILE=

obj: _xenocara_obj

includes:
	@cmp -s ${DIST}/include/GL/glu.h \
		${DESTDIR}${X11BASE}/include/GL/glu.h \
	|| ${INSTALL_DATA} ${DIST}/include/GL/glu.h \
		${DESTDIR}${X11BASE}/include/GL 

glu.pc: ${DIST}/src/glu/glu.pc.in Makefile
	sed -e 's,@INSTALL_DIR@,$(X11BASE),' \
	    -e 's,@INSTALL_LIB_DIR@,$(X11BASE)/lib,' \
	    -e 's,@INSTALL_INC_DIR@,$(X11BASE)/include,' \
	    -e 's,@LIB_DIR@,lib,' \
	    -e 's,@GLU_LIB@,GLU,' \
	    -e 's,@GLU_PC_LIB_PRIV@,,' \
	    -e 's,@GLU_PC_CFLAGS@,,' \
	    -e 's,@GLU_PC_REQ@,gl,' \
	    -e 's,@GLU_PC_REQ_PRIV@,,' \
	    -e 's,@VERSION@,$(MESA_MAJOR).$(MESA_MINOR).$(MESA_TINY),' \
	< ${DIST}/src/glu/glu.pc.in > glu.pc

afterinstall: glu.pc
	$(INSTALL) -c -m 644 -o root -g wheel glu.pc \
		${DESTDIR}${LIBDIR}/pkgconfig


.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${GLU}/libutil
.PATH: ${GLU}/libtess
.PATH: ${GLU}/libnurbs/interface
.PATH: ${GLU}/libnurbs/internals
.PATH: ${GLU}/libnurbs/nurbtess
