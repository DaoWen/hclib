export HCLIB_ROOT=$PWD/hclib-install-ocr
export LD_LIBRARY_PATH=${HCLIB_ROOT}/lib:${LD_LIBRARY_PATH}
[ -d "$HCLIB_ROOT" ] && echo "Setup OK for HCLIB on OCR"
if [ -z "$OCR_CONFIG" ]; then
    export OCR_CONFIG="$PWD/default.cfg"
    if ! [ -f "$OCR_CONFIG" ]; then
        curl "https://gist.githubusercontent.com/DaoWen/2badba27d099150cfdb6/raw" > "$OCR_CONFIG"
    fi
    echo "Set default OCR config for 8 workers"
fi
