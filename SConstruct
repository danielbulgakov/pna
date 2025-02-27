SOURCE_DIR = './src'
BUILD_DIR = './build'

import os

env = Environment(ENV=os.environ)

if env['PLATFORM'] == 'win32':
    env.Append(CCFLAGS=['/openmp'])
else:
    env.Append(CCFLAGS=['-fopenmp'], LINKFLAGS=['-fopenmp'])

SConscript(
    f'{SOURCE_DIR}/SConscript',
    variant_dir=BUILD_DIR,
    duplicate=0,
    exports={'env': env}
)