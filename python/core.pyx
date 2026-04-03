
from debug_mgr cimport decl
from libcpp.cast cimport reinterpret_cast
from libc.stdint cimport intptr_t
import ctypes
import os
import sys

cdef class Factory(object):

    @staticmethod
    def inst():
        cdef decl.IFactory *hndl = NULL
        cdef Factory factory
        global _inst

        if _inst is None:
            ext_dir = os.path.dirname(os.path.abspath(__file__))

            build_dir = os.path.abspath(os.path.join(ext_dir, "../../build"))

            # Determine platform-specific library name and build subdirs to search
            if sys.platform == "win32":
                libname = "debug-mgr.dll"
                # MSVC places output in Release/ or Debug/ subdirs; also check lib/ and root
                search_dirs = ("Release", "Debug", "lib", "")
            elif sys.platform == "darwin":
                libname = "libdebug-mgr.dylib"
                search_dirs = ("lib", "lib64")
            else:
                libname = "libdebug-mgr.so"
                search_dirs = ("lib", "lib64")

            # Search in the build directory first
            core_lib = None
            for libdir in search_dirs:
                candidate = (os.path.join(build_dir, libdir, libname)
                             if libdir else os.path.join(build_dir, libname))
                if os.path.isfile(candidate):
                    core_lib = candidate
                    break

            # Fall back to the installed package directory
            if core_lib is None:
                core_lib = os.path.join(ext_dir, libname)

            if not os.path.isfile(core_lib):
                raise Exception("Extension library core \"%s\" doesn't exist" % core_lib)

            so = ctypes.cdll.LoadLibrary(core_lib)

            func = so.debug_mgr_getFactory
            func.restype = ctypes.c_void_p

            hndl_v = func()
            hndl = <decl.IFactoryP>(<intptr_t>(hndl_v))

            factory = Factory()
            factory._hndl = hndl
            _inst = factory

        return _inst

    cpdef DebugMgr getDebugMgr(self):
        self._hndl.getDebugMgr()
        return DebugMgr.mk(self._hndl.getDebugMgr(), False)

_inst = None

cdef class DebugMgr(object):

    cpdef void enable(self, bool en):
        self._hndl.enable(en)

    cdef decl.IDebugMgr *getHndl(self):
        return self._hndl

    @staticmethod
    cdef DebugMgr mk(decl.IDebugMgr *hndl, bool owned=True):
        ret = DebugMgr()
        ret._hndl = hndl
        ret._owned = owned
        return ret
