SOURCE_DIR = './src'
BUILD_DIR = './build'

import os
import platform

env = Environment(ENV=os.environ)

if platform.system() == 'Windows':
    env.Append(CCFLAGS=['/openmp', '/EHsc'])
    env.Append(LINKFLAGS=['/SUBSYSTEM:CONSOLE'])
else: # For Linux
    env.Append(CCFLAGS=['-fopenmp'])
    env.Append(LINKFLAGS=['-fopenmp'])

SConscript(
    f'{SOURCE_DIR}/SConscript',
    variant_dir=BUILD_DIR,
    duplicate=0,
    exports={'env': env}
)