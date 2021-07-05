.include <bsd.xconf.mk>

PYTHON=		python3
DRM_SRC=	..

all:
	${PYTHON} ${DRM_SRC}/gen_table_fourcc.py \
	    ${DRM_SRC}/include/drm/drm_fourcc.h \
	    generated_static_table_fourcc.h

clean:
	rm -f generated_static_table_fourcc.h
