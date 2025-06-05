#	$OpenBSD: Makefile,v 1.11 2025/06/05 14:18:47 jsg Exp $

LIB=	mesa_util

NOPROFILE=

SRCS=	anon_file.c \
	bitscan.c \
	blob.c \
	build_id.c \
	cnd_monotonic.c \
	compress.c \
	thread_sched.c \
	crc32.c \
	dag.c \
	disk_cache.c \
	disk_cache_os.c \
	double.c \
	fast_idiv_by_const.c \
	fossilize_db.c \
	futex.c \
	half_float.c \
	hash_table.c \
	helpers.c \
	u_idalloc.c \
	log.c \
	memstream.c \
	mesa-sha1.c \
	mesa-blake3.c \
	os_file.c \
	os_file_notify.c \
	os_memory_fd.c \
	os_misc.c \
	os_socket.c \
	u_trace.c \
	os_time.c \
	ralloc.c \
	rand_xor.c \
	rb_tree.c \
	register_allocate.c \
	rgtc.c \
	set.c \
	simple_mtx.c \
	sha1.c \
	slab.c \
	softfloat.c \
	sparse_array.c \
	string_buffer.c \
	strtod.c \
	texcompress_astc_luts.cpp \
	texcompress_astc_luts_wrap.cpp \
	u_atomic.c \
	u_call_once.c \
	u_dl.c \
	u_dynarray.c \
	u_cpu_detect.c \
	u_printf.c \
	u_worklist.c \
	u_debug.c \
	u_debug_memory.c \
	u_math.c \
	u_mm.c \
	u_process.c \
	u_qsort.cpp \
	rwlock.c \
	u_hash_table.c \
	u_queue.c \
	u_thread.c \
	u_vector.c \
	vl_zscan_data.c \
	vma.c \
	mesa_cache_db.c \
	mesa_cache_db_multipart.c

SRCS+=	u_debug_stack.c \
	u_debug_symbol.c \
	format_srgb.c

SRCS+=	u_format.c \
	u_format_bptc.c \
	u_format_etc.c \
	u_format_fxt1.c \
	u_format_latc.c \
	u_format_other.c \
	u_format_rgtc.c \
	u_format_s3tc.c \
	u_format_tests.c \
	u_format_unpack_neon.c \
	u_format_yuv.c \
	u_format_zs.c

SRCS+=	u_format_table.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS} -ffunction-sections -fdata-sections
CPPFLAGS+=	-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/util/format \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util/format

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/util
.PATH: ${MESA_SRC}/src/util/format
.PATH: ${MESA_SRC}/src/util/indices
.PATH: ${MESA_SRC}/src/util/perf
.PATH: ${MESA_SRC}/src/util/sha1
.PATH: ${MESA_SRC}/generated/src/util
.PATH: ${MESA_SRC}/generated/src/util/format
