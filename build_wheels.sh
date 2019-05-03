#!/bin/bash
set -e -x

# Install a system package required by our library
yum install -y atlas-devel

/opt/python/cp36*/bin/pip install cmake
ln -s /opt/python/cp36*/bin/cmake /usr/sbin/cmake

# Compile wheels
for PYBIN in /opt/python/cp3*/bin; do
    "${PYBIN}/pip" install -r /io/dev-requirements.txt
    "${PYBIN}/pip" wheel /io/ -w wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in wheelhouse/*.whl; do
    auditwheel repair "$whl" --plat $PLAT -w /io/wheelhouse/
done