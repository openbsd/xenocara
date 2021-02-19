#	$OpenBSD: Makefile,v 1.2 2021/02/19 12:29:37 matthieu Exp $

LIB=	drm_amdgpu

SRCS=	amdgpu_asic_id.c \
	amdgpu_bo.c \
	amdgpu_cs.c \
	amdgpu_device.c \
	amdgpu_gpu_info.c \
	amdgpu_vamgr.c \
	amdgpu_vm.c \
	handle_table.c

.include "../Makefile.inc"

CPPFLAGS+=	-I${DRM_SRC}/amdgpu \
		-DAMDGPU_ASIC_ID_TABLE=\"${X11BASE}/share/libdrm/amdgpu.ids\"

LDADD+= -L${.CURDIR}/../libdrm/${__objdir} -ldrm

obj: _xenocara_obj

beforeinstall:
	rm -f ${DESTDIR}${LIBDIR}/lib${LIB}.la
	${INSTALL} ${INSTALL_COPY} -o ${SHAREOWN} -g ${SHAREGRP} \
	    -m ${SHAREMODE} ${DRM_SRC}/data/amdgpu.ids \
	    ${DESTDIR}/${X11BASE}/share/libdrm/amdgpu.ids

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${DRM_SRC}/amdgpu
