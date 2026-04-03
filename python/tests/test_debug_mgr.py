"""
Tests for the debug_mgr Python package.

These tests validate that the shared library is correctly discovered and that
the Factory and DebugMgr objects are functional across all platforms.
"""
import pytest


def test_import():
    """The core extension module must import without error."""
    import debug_mgr.core


def test_factory_singleton():
    """Factory.inst() must return a non-None object and be a singleton."""
    from debug_mgr.core import Factory

    f1 = Factory.inst()
    f2 = Factory.inst()

    assert f1 is not None
    assert f1 is f2


def test_get_debug_mgr():
    """Factory.getDebugMgr() must return a non-None DebugMgr instance."""
    from debug_mgr.core import Factory

    factory = Factory.inst()
    dm = factory.getDebugMgr()
    assert dm is not None


def test_enable_disable():
    """DebugMgr.enable() must be callable with True and False without raising."""
    from debug_mgr.core import Factory

    factory = Factory.inst()
    dm = factory.getDebugMgr()
    dm.enable(True)
    dm.enable(False)
