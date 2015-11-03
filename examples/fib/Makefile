TARGET := fib

$(TARGET): $(TARGET).c
	@ [ -d "${HCLIB_ROOT}" ] || (echo Need to set HCLIB_ROOT; exit 1)
	gcc $^ -O0 -g -o$@ -pthread -std=gnu99 -I${HCLIB_ROOT}/include ${HCLIB_ROOT}/lib/libhclib.a

WORKLOAD_ARGS ?= 5

.PHONY: run
run: $(TARGET)
	./$(TARGET) $(WORKLOAD_ARGS)

clean:
	rm -f $(TARGET)
