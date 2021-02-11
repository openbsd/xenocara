.include <bsd.xconf.mk>
.include <bsd.own.mk>

CFLAGS+= \
	-std=gnu99 \
	-fvisibility=hidden \
	-Wall \
	-Wdeclaration-after-statement \
	-Werror=implicit-function-declaration \
	-Werror=undef \
	-Wextra \
	-Winit-self \
	-Winline \
	-Winvalid-pch \
	-Wmissing-declarations \
	-Wmissing-format-attribute \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Wno-attributes \
	-Wno-long-long \
	-Wno-missing-field-initializers \
	-Wno-unused-parameter \
	-Wold-style-definition \
	-Wpacked \
	-Wpointer-arith \
	-Wshadow \
	-Wsign-compare \
	-Wstrict-aliasing=2 \
	-Wstrict-prototypes \
	-Wswitch-enum \
	-Wwrite-strings

CPPFLAGS+= \
	-DHAVE_ALLOCA_H=0 \
	-DHAVE_CAIRO=0 \
	-DHAVE_EXYNOS=0 \
	-DHAVE_FREEDRENO_KGSL=0 \
	-DHAVE_LIBDRM_ATOMIC_PRIMITIVES=1 \
	-DHAVE_LIB_ATOMIC_OPS=0 \
	-DHAVE_NOUVEAU=0 \
	-DHAVE_OPEN_MEMSTREAM=1 \
	-DHAVE_RADEON=1 \
	-DHAVE_SYS_SELECT_H=1 \
	-DHAVE_SYS_SYSCTL_H=1 \
	-DHAVE_VALGRIND=0 \
	-DHAVE_VC4=0 \
	-DHAVE_VISIBILITY=1 \
	-DHAVE_VMWGFX=0 \
	-DUDEV=0

.if ${MACHINE} == "amd64" || ${MACHINE} == "i386"
WITH_INTEL=yes
CPPFLAGS+= -DHAVE_INTEL=1
.else
WITH_INTEL=no
CPPFLAGS+= -DHAVE_INTEL=0
.endif
