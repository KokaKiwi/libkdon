import waflib
from glob2 import glob

OFLAGS = '-ggdb3'

APPNAME = 'kdon'

top = '.'
out = 'build'

INCLUDE_DIRS = [
    'include',
    out
]

class test(waflib.Build.BuildContext):
    cmd = 'test'

def options(opt):
    opt.load('compiler_c')

    opt.add_option('--debug', action = 'store_true', default = False, help = 'Enable debugging.')

def find_package(conf, package, name, required = False):
    return conf.check_cfg(package      = package,
                          args         = ['--cflags', '--libs'],
                          uselib_store = name,
                          mandatory    = required)

def configure(conf):
    conf.load('compiler_c')

    conf.env.append_unique('CFLAGS', OFLAGS)

    find_package(conf, 'glib-2.0', 'glib2', True)
    jansson = find_package(conf, 'jansson', 'jansson')
    msgpack = find_package(conf, 'msgpack', 'msgpack')
    libxml2 = find_package(conf, 'libxml-2.0', 'libxml2')
    libyaml = find_package(conf, 'yaml-0.1', 'libyaml')

    conf.env.HAVE_JANSSON = jansson != None
    conf.env.HAVE_MSGPACK = msgpack != None
    conf.env.HAVE_LIBXML2 = libxml2 != None
    conf.env.HAVE_LIBYAML = libyaml != None

    if conf.options.debug:
        conf.define('DEBUG', conf.options.debug)

    conf.write_config_header('kdon_config.h')

def build(bld):
    libcencoder_sources = glob('src/*.c')
    use_libs = ['glib2']

    if bld.env.HAVE_JANSSON:
        libcencoder_sources.append('src/encode/json.c')
        libcencoder_sources.append('src/decode/json.c')
        use_libs.append('jansson')

    if bld.env.HAVE_MSGPACK:
        libcencoder_sources.append('src/encode/msgpack.c')
        use_libs.append('msgpack')

    if bld.env.HAVE_LIBXML2:
        libcencoder_sources.append('src/encode/xml.c')
        use_libs.append('libxml2')

    if bld.env.HAVE_LIBYAML:
        libcencoder_sources.append('src/encode/yaml.c')
        use_libs.append('libyaml')

    libcencoder = bld.stlib(source = libcencoder_sources,
                            features = 'c',
                            target = 'kdon',
                            name = 'libkdon',
                            includes = INCLUDE_DIRS,
                            use = use_libs)

    if bld.cmd == 'test':
        bld.program(source = 'tests/test_build.c',
                    features = 'c',
                    target = 'kdon_test_build',
                    includes = INCLUDE_DIRS,
                    linkflags = [OFLAGS],
                    use = ['libkdon'] + use_libs)
        bld.program(source = 'tests/test_load.c',
                    features = 'c',
                    target = 'kdon_test_load',
                    includes = INCLUDE_DIRS,
                    linkflags = [OFLAGS],
                    use = ['libkdon'] + use_libs)
