if CRT_ENABLED

RUNTIME_CRT_SRC=crt

noinst_LTLIBRARIES += libruntime_crt.la
libhclib_la_LIBADD += libruntime_crt.la

RT_CRT_LDFLAGS=$(CRT_LDFLAGS) $(CRT_LIBS) -lpthread
RT_CRT_CFLAGS=-I$(CRT_INSTALL)/include

libruntime_crt_la_SOURCES =              \
$(RUNTIME_CRT_SRC)/deque.c               \
$(RUNTIME_CRT_SRC)/runtime-support-crt.c \
$(RUNTIME_CRT_SRC)/runtime-crt.c

if X86
if OSX
libruntime_crt_la_SOURCES += \
fcontext/jump_i386_sysv_macho_gas.S \
fcontext/make_i386_sysv_macho_gas.S
else
if LINUX
libruntime_crt_la_SOURCES += \
fcontext/jump_i386_sysv_elf_gas.S \
fcontext/make_i386_sysv_elf_gas.S
else
$(error Unknown OS)
endif # LINUX
endif # OSX
else
if X86_64
if OSX
libruntime_crt_la_SOURCES += \
fcontext/jump_x86_64_sysv_macho_gas.S \
fcontext/make_x86_64_sysv_macho_gas.S
else
if LINUX
libruntime_crt_la_SOURCES += \
fcontext/jump_x86_64_sysv_elf_gas.S \
fcontext/make_x86_64_sysv_elf_gas.S
else
$(error Unknown OS)
endif # LINUX
endif # OSX
else
$(error Unknown architecture)
endif # X86_64
endif # X86


libruntime_crt_la_CFLAGS = $(CFLAGS) $(INCLUDES_DIR) $(RT_CRT_CFLAGS)
libruntime_crt_la_LDFLAGS = $(RT_CRT_LDFLAGS)

endif
