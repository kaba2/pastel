import sys

sys.path.insert(0, './scripts')
from build import ConfigManager, sdl_config

env = Environment()
env.EnsureSConsVersion(0,97)

config = ConfigManager('config.py')
sdl_config(env, config)

env.Append(CXXFLAGS=' -Wall -Wno-sign-compare -O2');

Export('env')
Export('config')

objs = env.SConscript('pastel/SConscript')

libenv = env.Clone()
config.apply('SDL', libenv)
config.apply('RPly', libenv)
config.apply('OpenGL', libenv)
config.apply('GLEW', libenv)
libenv.SharedLibrary(target='pastel', source=objs)

env.SConscript('test/SConscript')
