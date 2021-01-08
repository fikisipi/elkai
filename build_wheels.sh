#!/bin/bash
set -e -x

# Install a system package required by our library
yum install -y atlas-devel

MPATH="$PATH"

# Don't need this hack anymore!
if [[ 0 -eq 1 ]]; then
# Reuse 3.7 CMake for 3.8
for PYBIN in /opt/python/cp37*/bin; do
    "${PYBIN}/python" -m pip install cmake
    cp -r /opt/python/cp37*/lib/python3.7/site-packages/cmake /opt/python/cp38-cp38/lib/python3.8/site-packages/
    cp /opt/python/cp37*/bin/cmake /opt/python/cp38-cp38/bin
done
fi

# Compile wheels
for PYBIN in /opt/python/cp3*/bin; do
    PATH="${PYBIN}/:${MPATH}"
    #if [[ $PYBIN != *"cp38"* ]]; then
    "${PYBIN}/python" -m pip install cmake
    #fi
    "${PYBIN}/python" -m pip install scikit-build ninja
    "${PYBIN}/python" -m pip wheel /io/ -w dist/
    /bin/rm -rf /io/_skbuild
    /bin/rm -rf /io/CMakeFiles || true
done

# Bundle external shared libraries into the wheels
PLAT="manylinux1_x86_64"
for whl in dist/*.whl; do
    auditwheel repair "$whl" --plat $PLAT -w /io/dist/
done
