#!@PYTHON_EXECUTABLE@
#ckwg +4
# Copyright 2012-2013 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


def test_import():
    try:
        import sprokit.pipeline.version
    except:
        test_error("Failed to import the version module")


def test_api_calls():
    from sprokit.pipeline import version

    version.compile.major
    version.compile.minor
    version.compile.patch
    version.compile.version_string
    version.compile.git_build
    version.compile.git_hash
    version.compile.git_hash_short
    version.compile.git_dirty
    version.compile.check(0, 0, 0)

    version.runtime.major
    version.runtime.minor
    version.runtime.patch
    version.runtime.version_string
    version.runtime.git_build
    version.runtime.git_hash
    version.runtime.git_hash_short
    version.runtime.git_dirty
    version.runtime.check(0, 0, 0)


if __name__ == '__main__':
    import os
    import sys

    if not len(sys.argv) == 4:
        test_error("Expected three arguments")
        sys.exit(1)

    testname = sys.argv[1]

    os.chdir(sys.argv[2])

    sys.path.append(sys.argv[3])

    tests = \
        { 'import': test_import
        , 'api_calls': test_api_calls
        }

    from sprokit.test.test import *

    run_test(testname, tests)