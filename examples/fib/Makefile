TARGET := fib

$(TARGET): $(TARGET).c
	@ [ -d "${HCLIB_ROOT}" ] || (echo Need to set HCLIB_ROOT; exit 1)
	gcc $^ -O0 -g -o$@ -pthread -std=gnu99 -I.. -I${HCLIB_ROOT}/include ${HCLIB_ROOT}/lib/libhclib.a

WORKLOAD_ARGS ?= 5

.PHONY: scale
scale: SETUP_ENV=LD_PRELOAD="/usr/lib/libtcmalloc.so.4"
scale: run

.PHONY: run
run: $(TARGET)
	$(SETUP_ENV) ./$(TARGET) $(WORKLOAD_ARGS)

clean:
	rm -f $(TARGET)
