TARGET := producer-consumer

$(TARGET): $(TARGET).c
	@ [ -d "${HCLIB_ROOT}" ] || (echo Need to set HCLIB_ROOT; exit 1)
	gcc -o$@ -std=gnu99 -I${HCLIB_ROOT}/include -L${HCLIB_ROOT}/lib -lhclib $^

WORKLOAD_ARGS ?= 

.PHONY: run
run: $(TARGET)
	./$(TARGET) $(WORKLOAD_ARGS)

clean:
	rm -f $(TARGET)
