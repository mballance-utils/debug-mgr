#****************************************************************************
#* setup.py for debug-mgr
#****************************************************************************
import os
import subprocess
import sys
from setuptools import setup
from distutils.extension import Extension
from setuptools.command.build_ext import build_ext as _build_ext
from distutils.file_util import copy_file

if "-DDEBUG" in sys.argv:
    _DEBUG = True
    sys.argv.remove("-DDEBUG")
else:
    _DEBUG = False

_DEBUG_LEVEL = 0

version="0.0.1"

if "BUILD_NUM" in os.environ.keys():
    version += ".%s" % os.environ["BUILD_NUM"]

if "CMAKE_BUILD_TOOL" in os.environ.keys():
    cmake_build_tool = os.environ["CMAKE_BUILD_TOOL"]
else:
    cmake_build_tool = "Ninja"

# First need to establish where things are
debug_mgr_dir = os.path.dirname(os.path.abspath(__file__))

if os.path.isdir(os.path.join(debug_mgr_dir, "packages")):
    print("Packages are inside this directory")
    packages_dir = os.path.join(debug_mgr_dir, "packages")
else:
    parent = os.path.dirname(debug_mgr_dir)
    
    if os.path.isdir(os.path.join(parent, "debug-mgr")):
        print("debug-mgr is a peer")
        packages_dir = parent
    else:
        raise Exception("Unexpected source layout")

# Now, build the core tblink-rpc-hdl library
cwd = os.getcwd()
if not os.path.isdir(os.path.join(cwd, "build")):
    os.makedirs(os.path.join(cwd, "build"))

if _DEBUG:
    BUILD_TYPE = "-DCMAKE_BUILD_TYPE=Debug"
else:
    BUILD_TYPE = "-DCMAKE_BUILD_TYPE=Release"

env = os.environ.copy()
python_bindir = os.path.dirname(sys.executable)
print("python_bindir: %s" % str(python_bindir))

if "PATH" in env.keys():
    env["PATH"] = python_bindir + os.pathsep + env["PATH"]
else:
    env["PATH"] = python_bindir

# Run configure...
result = subprocess.run(
    ["cmake", 
     debug_mgr_dir,
     "-G%s" % cmake_build_tool,
     BUILD_TYPE,
     "-DPACKAGES_DIR=%s" % packages_dir,
     "-DCMAKE_INSTALL_PREFIX=%s" % os.path.join(cwd, "build"),
     "-DCMAKE_OSX_ARCHITECTURES='x86_64;arm64'",
     ],
    cwd=os.path.join(cwd, "build"),
    env=env)

if result.returncode != 0:
    raise Exception("cmake configure failed")

if cmake_build_tool == "Ninja":
    result = subprocess.run(
        ["ninja",
         "-j",
         "%d" % os.cpu_count()
        ],
        cwd=os.path.join(cwd, "build"),
        env=env)
elif cmake_build_tool == "Unix Makefiles":
    result = subprocess.run(
        ["make",
         "-j%d" % os.cpu_count()
        ],
        cwd=os.path.join(cwd, "build"),
        env=env)
else:
    raise Exception("Unknown make system %s" % cmake_build_tool)

if result.returncode != 0:
    raise Exception("build failed")

if cmake_build_tool == "Ninja":
    result = subprocess.run(
        ["ninja",
         "-j",
         "%d" % os.cpu_count(),
         "install"
        ],
        cwd=os.path.join(cwd, "build"),
        env=env)
elif cmake_build_tool == "Unix Makefiles":
    result = subprocess.run(
        ["make",
         "-j%d" % os.cpu_count(),
         "install"
        ],
        cwd=os.path.join(cwd, "build"),
        env=env)
else:
    raise Exception("Unknown make system %s" % cmake_build_tool)

if result.returncode != 0:
    raise Exception("build failed")

extra_compile_args = []
if _DEBUG:
    extra_compile_args += ["-g", "-O0", "-DDEBUG=%s" % _DEBUG_LEVEL, "-UNDEBUG"]
else:
    extra_compile_args += ["-DNDEBUG", "-O3"]

if sys.platform == "darwin":
    extra_compile_args += ["-std=c++11"]

class build_ext(_build_ext):
    def run(self):
        super().run()

    # Needed for Windows to not assume python module (generate interface in def file)
    def get_export_symbols(self, ext):
        return None

    def copy_extensions_to_source(self):
        """ Like the base class method, but copy libs into proper directory in develop. """
        print("copy_extensions_to_source")
        super().copy_extensions_to_source()

        build_py = self.get_finalized_command("build_py")
        
        ext = self.extensions[0]
        fullname = self.get_ext_fullname(ext.name)
        filename = self.get_ext_filename(fullname)
        modpath = fullname.split(".")
        package = ".".join(modpath[:-1])
        package_dir = build_py.get_package_dir(package)

        if sys.platform == "darwin":
            ext = ".dylib"
        elif sys.platform == "win32":
            ext = ".dll"
        else:
            ext = ".so"

        if sys.platform == "win32":
            pref = ""
        else:
            pref = "lib"

        copy_file(
            os.path.join(cwd, "build", "src", "%sdebug-mgr%s" % (pref, ext)),
            os.path.join(package_dir, "%sdebug-mgr%s" % (pref, ext)))
        
        if sys.platform == "win32":
            copy_file(
                os.path.join(cwd, "build", "src", "debug-mgr.lib" ),
                os.path.join(package_dir, "debug-mgr.lib" % (pref, ext)))

        dest_filename = os.path.join(package_dir, filename)
        
        print("package_dir: %s dest_filename: %s" % (package_dir, dest_filename))

print("extra_compile_args=" + str(extra_compile_args))

if sys.platform == "darwin":
    libext = ".dylib"
    libpref = "lib"
elif sys.platform == "win32":
    libext = ".dll"
    libpref = "lib"
else:
    libext = ".so"
    libpref = "lib"

if sys.platform == "win32":
    package_data = [
        "debug-mgr.dll",
        "debug-mgr.lib"
    ]
else:
    package_data = [
        "%sdebug-mgr.%s" % (libpref, libext)
    ]

ext = Extension("debug_mgr.core",
            extra_compile_args=extra_compile_args,
            sources=[
                os.path.join(debug_mgr_dir, 'python', "core.pyx"), 
            ],
            language="c++",
            include_dirs=[
                os.path.join(debug_mgr_dir, 'src'),
                os.path.join(debug_mgr_dir, 'src', 'include')
            ]
        )
ext.cython_directives={'language_level' : '3'}

setup(
  name = "debug-mgr",
  version=version,
  packages=['debug_mgr'],
  package_dir = {'' : 'python'},
  package_data={ 'debug_mgr': package_data },
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Simple debug manager for use of C++ Python extensions"),
  long_description="""
  Simple debug manager for use of C++ Python extensions
  """,
  license = "Apache 2.0",
  keywords = ["Debug", "C/C++", "Python"],
  url = "https://github.com/mballance/debug-mgr",
  install_requires=[
  ],
  setup_requires=[
    'setuptools_scm',
    'cython'
  ],
  cmdclass={'build_ext': build_ext},
  ext_modules=[ ext ]
)


