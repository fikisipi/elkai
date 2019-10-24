#!/bin/bash
set -e -x

# Install a system package required by our library
yum install -y atlas-devel

MPATH="$PATH"
# Compile wheels
for PYBIN in /opt/python/cp3*/bin; do
    PATH="${PYBIN}/:${MPATH}"
    "{PYBIN}/pip" install --upgrade pip
    "{PYBIN}/python" -c 'import sys; from pip._internal import main; sys.exit(main.main())' install scikit-build cmake ninja
    "{PYBIN}/python" -c 'import sys; from pip._internal import main; sys.exit(main.main())' wheel /io/ -w dist/
    /bin/rm -rf /io/_skbuild
    /bin/rm -rf /io/CMakeFiles || true
done

# Bundle external shared libraries into the wheels
PLAT="manylinux1_x86_64"
for whl in dist/*.whl; do
    auditwheel repair "$whl" --plat $PLAT -w /io/dist/
done
