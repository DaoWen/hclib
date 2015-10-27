TARGET := deadlock

$(TARGET): $(TARGET).c
	@ [ -d "${HCLIB_ROOT}" ] || (echo Need to set HCLIB_ROOT; exit 1)
	gcc -o$@ -std=gnu99 -I${HCLIB_ROOT}/include -L${HCLIB_ROOT}/lib -lhclib $^

NPROC ?= 4

.PHONY: run
run: $(TARGET)
	./$(TARGET) -nproc $(NPROC)

clean:
	rm -f $(TARGET)
