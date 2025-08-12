CPPFLAGS += -DHAVE_CONFIG_H -DPIXMAN_NO_TLS
CPPFLAGS += -I${TOP}/test/utils -I${TOP}/pixman -I${TOP}/mk
LDADD += -Wl,-rpath=../.. -L../.. -lpixman-1 -lm
DPADD += ../../libpixman-1.so.46.4

obj: _xenocara_obj

.if !target(test)
test:
	@time -p ./${PROG}
.endif

NOINSTALL=
NOMAN=

.PATH: ${TOP}/test
