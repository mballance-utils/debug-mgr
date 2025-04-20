#****************************************************************************
#* setup.py for debug-mgr
#****************************************************************************
import os
import subprocess
import sys
import platform
from setuptools import Extension, find_namespace_packages

#if "IVPM_PYTHONPATH" in os.environ.keys():
#    ps = ";" if platform.system() == "Windows" else ":"
#    for i,p in enumerate(os.environ["IVPM_PYTHONPATH"].split(ps)):
#        sys.path.insert(i, p)

version="0.0.2"

proj_dir = os.path.dirname(os.path.abspath(__file__))

try:
    import sys
    print("proj_dir: %s" % proj_dir)
    sys.path.insert(0, os.path.join(proj_dir, "python"))
    from debug_mgr.__build_num__ import BUILD_NUM
    version += ".%s" % str(BUILD_NUM)
    print("import build_num: version=%s" % version)
except ImportError as e:
    print("failed to import build_num: %s" % str(e))

isSrcBuild = False

try:
    from ivpm.setup import setup
    isSrcBuild = os.path.isdir(os.path.join(proj_dir, "src"))
    print("debug-mgr: running IVPM")
except ImportError as e:
    # We're running in an independent environment
    from setuptools import setup
    print("debug-mgr: running setuptools: %s" % str(e))

if isSrcBuild:
    incdir = os.path.join(proj_dir, "src", "include")
else:
    incdir = os.path.join(proj_dir, "python/debug_mgr/share/include")

ext = Extension("debug_mgr.core",
            sources=[
                os.path.join(proj_dir, 'python', "core.pyx"), 
            ],
            language="c++",
            include_dirs=[incdir])
ext.cython_directives={'language_level' : '3'}

setup_requires=['setuptools_scm', 'cython', 'ivpm']

#if isSrcBuild:
#    setup_requires.append('ivpm')
#else:
#    setup_requires.append('ivpm')

setup_args = dict(
  name="debug-mgr",
  version=version,
  packages=find_namespace_packages(where='python'),
  package_dir = {'' : 'python'},
  package_data={ 'debug_mgr': [
      "core.pxd",
      "decl.pxd" ]},
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Simple debug manager for use of C++ Python extensions"),
  long_description="""
  Simple debug manager for use of C++ Python extensions
  """,
  license = "Apache 2.0",
  keywords = ["Debug", "C/C++", "Python"],
  url = "https://github.com/mballance/debug-mgr",
  install_requires=[],
  setup_requires=setup_requires,
  entry_points={
    "ivpm.pkginfo": [
        'debug-mgr = debug_mgr.pkginfo:PkgInfo'
    ],
    'console_scripts': [
      'debug-mgr = debug_mgr.__main__:main'
    ]
  },
  ext_modules=[ ext ],
)

if isSrcBuild:
  setup_args["ivpm_extra_data"] = {
      "debug_mgr": [
          ("src/include", "share"),
          ("build/{libdir}/{libpref}debug-mgr{dllext}", ""),
      ]
  }

setup(**setup_args)


