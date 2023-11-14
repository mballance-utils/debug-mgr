
from debug_mgr cimport decl
from libcpp.cast cimport reinterpret_cast
from libc.stdint cimport intptr_t
import ctypes
import os

cdef class Factory(object):

    @staticmethod
    def inst():
        cdef decl.IFactory *hndl = NULL
        cdef Factory factory
        global _inst

        if _inst is None:
            ext_dir = os.path.dirname(os.path.abspath(__file__))

            build_dir = os.path.abspath(os.path.join(ext_dir, "../../build"))

            # First, look in the build directory
            core_lib = None
            libname = "libdebug-mgr.so"
            for libdir in ("lib", "lib64"):
                if os.path.isfile(os.path.join(build_dir, libdir, libname)):
                    core_lib = os.path.join(build_dir, libdir, libname)
                    break
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
