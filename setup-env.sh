export HCLIB_ROOT=$PWD/hclib-install
export LD_LIBRARY_PATH=${HCLIB_ROOT}/lib:${LD_LIBRARY_PATH}
[ -d "$HCLIB_ROOT" ] && echo Setup OK for HCLIB on CRT
