"""
Pytest configuration and fixtures for libfontenc tests.

These tests exercise the encoding file parser (encparse.c), specifically
the setCode() function, by crafting .enc files and loading them through
a C test helper that wraps the static parseEncodingFile().

When run via meson (``meson test``), the helper library is pre-built and
its path is passed in the ``TEST_HELPER_LIB`` environment variable.
When run directly (``pytest test/``), the helper is compiled on the fly
from the source tree.
"""

import ctypes
import gzip
import os
import pathlib
import subprocess

import pytest

SRCDIR = pathlib.Path(__file__).parent.parent


def _compile_helper(tmpdir):
    """Compile test_helper.c into a shared library for direct pytest runs."""
    helper_so = tmpdir / "test_helper.so"
    builddir = SRCDIR / "build"
    libdir = builddir if (builddir / "libfontenc.so.1.0.0").exists() else None

    if libdir is None:
        pytest.skip("Library not built; run 'meson setup build && ninja -C build' first")

    cmd = [
        "cc",
        "-shared", "-fPIC", "-g", "-O0",
        f"-I{SRCDIR}/include",
        f"-I{SRCDIR}/src",
        f"-I{builddir}",
        "-DHAVE_CONFIG_H",
        '-DFONT_ENCODINGS_DIRECTORY="/dev/null"',
        str(SRCDIR / "test" / "test_helper.c"),
        "-o", str(helper_so),
        "-lz",
        f"-L{libdir}",
        "-lfontenc",
        f"-Wl,-rpath,{libdir}",
    ]

    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        pytest.fail(f"Failed to compile test helper:\n{result.stderr}")

    return helper_so


@pytest.fixture(scope="session")
def test_helper_lib(tmp_path_factory):
    """
    Return the path to the test helper shared library.

    Uses the meson-built library if TEST_HELPER_LIB is set, otherwise
    compiles it from source.
    """
    env_path = os.environ.get("TEST_HELPER_LIB")
    if env_path:
        path = pathlib.Path(env_path)
        if path.exists():
            return path
        pytest.fail(f"TEST_HELPER_LIB points to non-existent file: {env_path}")

    tmpdir = tmp_path_factory.mktemp("helper")
    return _compile_helper(tmpdir)


@pytest.fixture(scope="session")
def parse_encoding(test_helper_lib):
    """
    Return a function that parses an encoding file and returns
    (success: bool, row_size: int or None).

    success is True if the file parsed without error.
    """
    lib = ctypes.CDLL(str(test_helper_lib))
    lib.test_parse_encoding_file.restype = ctypes.c_int
    lib.test_parse_encoding_file.argtypes = [
        ctypes.c_char_p,
        ctypes.POINTER(ctypes.c_int),
    ]

    def _parse(filepath):
        row_size = ctypes.c_int(0)
        ret = lib.test_parse_encoding_file(
            str(filepath).encode(),
            ctypes.byref(row_size),
        )
        if ret == 1:
            return True, row_size.value
        elif ret == 0:
            return False, None
        else:
            return None, None  # file open error

    return _parse


@pytest.fixture
def enc_file(tmp_path):
    """
    Return a factory function that creates a gzipped .enc file with
    the given content and returns its path.
    """
    counter = [0]

    def _create(content, name=None):
        if name is None:
            counter[0] += 1
            name = f"test-{counter[0]}.enc.gz"
        path = tmp_path / name
        with gzip.open(path, "wt") as f:
            f.write(content)
        return path

    return _create
