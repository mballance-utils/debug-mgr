#!/bin/sh -x

git config --global --add safe.directory /io

# BUILD_NUM is the PEP 440 suffix the workflow computed, e.g.
# "dev33020374597+gh.g7775f37". It is EMPTY on a tag build, and an empty file
# is not the same as an absent one -- setup.py keys off the import failing.
if [ -n "${BUILD_NUM}" ]; then
    echo "BUILD_NUM=\"${BUILD_NUM}\"" > python/debug_mgr/__build_num__.py
else
    rm -f python/debug_mgr/__build_num__.py
fi
${IVPM_PYTHON} -m pip install ivpm cython setuptools --pre
# -d default, explicitly: without it ivpm resolves default-dev on a source
# checkout and drags the C++ test dependencies into the release build path.
${IVPM_PYTHON} -m ivpm update -a -d default --py-prerls-packages --py-pip

PYTHON=./packages/python/bin/python
${PYTHON} -m pip install twine auditwheel ninja wheel cython
${PYTHON} setup.py bdist_wheel

for whl in dist/*.whl; do
    ${PYTHON} -m auditwheel repair --only-plat $whl
    rm $whl
done

${PYTHON} -m pip install wheelhouse/*.whl
${PYTHON} -m pytest --import-mode=importlib python/tests/ -v
