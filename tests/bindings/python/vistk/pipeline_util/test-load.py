#!@PYTHON_EXECUTABLE@
#ckwg +4
# Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


def test_import():
    try:
        import vistk.pipeline_util.load
    except:
        test_error("Failed to import the load module")


def test_create():
    from vistk.pipeline_util import load

    load.Token()
    load.ConfigFlag()
    load.ConfigFlags()
    load.ConfigProvider()
    load.ConfigKeyOptions()
    load.ConfigKey()
    load.ConfigValue()
    load.ConfigValues()
    load.MapOptions()
    load.InputMap()
    load.OutputMap()
    load.ConfigBlock()
    load.ProcessBlock()
    load.ConnectBlock()
    load.GroupSubblock()
    load.GroupSubblocks()
    load.GroupBlock()
    load.PipeBlock()
    load.PipeBlocks()


def test_api_calls():
    from vistk.pipeline import config
    from vistk.pipeline import process
    from vistk.pipeline import process_registry
    from vistk.pipeline_util import load

    o = load.ConfigKeyOptions()
    o.flags
    o.provider
    o.flags = load.ConfigFlags()
    o.provider = load.ConfigProvider()

    o = load.ConfigKey()
    o.key_path
    o.options
    o.key_path = config.ConfigKeys()
    o.options = load.ConfigKeyOptions()

    o = load.ConfigValue()
    o.key
    o.value
    o.key = load.ConfigKey()
    o.value = config.ConfigValue()

    o = load.MapOptions()
    o.flags
    o.flags = process.PortFlags()

    o = load.InputMap()
    o.options
    o.from_
    o.to
    o.options = load.MapOptions()
    o.from_ = process.Port()
    o.to = process.PortAddr()

    o = load.OutputMap()
    o.options
    o.from_
    o.to
    o.options = load.MapOptions()
    o.from_ = process.PortAddr()
    o.to = process.Port()

    o = load.ConfigBlock()
    o.key
    o.values
    o.key = config.ConfigKeys()
    o.values = load.ConfigValues()

    o = load.ProcessBlock()
    o.name
    o.type
    o.config_values
    o.name = process.ProcessName()
    o.type = process.ProcessType()
    o.config_values = load.ConfigValues()

    o = load.ConnectBlock()
    o.from_
    o.to
    o.from_ = process.PortAddr()
    o.to = process.PortAddr()

    o = load.GroupSubblock()
    o.config = load.ConfigValue()
    o.config
    o.input = load.InputMap()
    o.input
    o.output = load.OutputMap()
    o.output

    o = load.GroupBlock()
    o.name
    o.subblocks
    o.name = process.ProcessName()
    o.subblocks = load.GroupSubblocks()

    o = load.PipeBlock()
    o.config = load.ConfigBlock()
    o.config
    o.process = load.ProcessBlock()
    o.process
    o.connect = load.ConnectBlock()
    o.connect
    o.group = load.GroupBlock()
    o.group


def test_simple_pipeline(path):
    from vistk.pipeline_util import load

    blocks = load.load_pipe_file(path)
    with open(path, 'r') as fin:
        load.load_pipe(fin)


def main(testname, path):
    if testname == 'import':
        test_import()
    elif testname == 'create':
        test_create()
    elif testname == 'api_calls':
        test_api_calls()
    elif testname == 'simple_pipeline':
        test_simple_pipeline(path)
    else:
        test_error("No such test '%s'" % testname)


if __name__ == '__main__':
    import os
    import sys

    if not len(sys.argv) == 5:
        test_error("Expected four arguments")
        sys.exit(1)

    testname = sys.argv[1]

    os.chdir(sys.argv[2])

    sys.path.append(sys.argv[3])

    pipeline_dir = sys.argv[4]

    path = os.path.join(pipeline_dir, '%s.pipe' % testname)

    from vistk.test.test import *

    try:
        main(testname, path)
    except BaseException as e:
        test_error("Unexpected exception: %s" % str(e))