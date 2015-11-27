TARGET := fib

$(TARGET): $(TARGET).c
	@ [ -d "${HCLIB_ROOT}" ] || (echo Need to set HCLIB_ROOT; exit 1)
	gcc $^ -O0 -g -o$@ -pthread -std=gnu99 -I.. -I${HCLIB_ROOT}/include ${HCLIB_ROOT}/lib/libhclib.a

WORKLOAD_ARGS ?= 5

.PHONY: run
run: $(TARGET)
	$(SETUP_ENV) ./$(TARGET) -nproc 8 $(WORKLOAD_ARGS)

.PHONY: hoard
hoard: SETUP_ENV=LD_PRELOAD="/opt/local/Hoard/lib/libhoard.so"
hoard: run

.PHONY: tbbmalloc
tbbmalloc: SETUP_ENV=LD_PRELOAD="/usr/lib/libtbbmalloc_proxy.so"
tbbmalloc: run

.PHONY: tcmalloc
tcmalloc: SETUP_ENV=LD_PRELOAD="/usr/lib/libtcmalloc.so.4"
tcmalloc: run

.PHONY: jemalloc
jemalloc: SETUP_ENV=LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libjemalloc.so"
jemalloc: run

clean:
	rm -f $(TARGET)
