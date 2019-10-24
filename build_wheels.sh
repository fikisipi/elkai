#!/bin/bash
set -e -x

# Install a system package required by our library
yum install -y atlas-devel cmake

MPATH="$PATH"
# Compile wheels
for PYBIN in /opt/python/cp3*/bin; do
    PATH="${PYBIN}/:${MPATH}"
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
