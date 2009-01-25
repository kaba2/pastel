import sys
import glob

# class ConfigManager
#
# The configuration manager class.  Reads the given file to retrieve
# configuration variable values.
# 
class ConfigManager:
	def __init__(self, filename):
		self.__libs = {}

		ns = {}
		execfile(filename, ns, ns)
		del ns['__builtins__']
		for k,v in ns.items():
			libname, key = k.split('_', 1)
			try:
				lib = self.__libs[libname]
			except KeyError:
				self.__libs[libname] = {}
				lib = self.__libs[libname]

			if v:
				if key.find('FLAGS') != -1 or not isinstance(v, str):
					value = v
				else:
					value = v.split()
				
				try:
					lib[key] += value
				except KeyError:
					lib[key] = value

	def __getitem__(self, key):
		return self.__libs[key]

	def append(self, libname, **kwargs):
		try:
			lib = self.__libs[libname]
		except KeyError:
			lib = self.__libs[libname] = {}
			
		for k,v in kwargs.items():
			try:
				lib[k] += v
			except KeyError:
				lib[k] = v

	def apply(self, libname, env):
		env.Append(**self.__libs[libname])


# sdl_config()
#
# Calls sdl-config to retrieve the SDL flags.  Uses the variable
# SDL_CONFIG in configuration manager to determine which program to
# call.  If the variable is set to False, does nothing.  If it's set
# to True, expects sdl-config to be in path.  Otherwise expects that
# the SDL_CONFIG variable is set to the absolute path of sdl-config.
#
# PARAMETERS
#   env       SCons environment
#   config    configuration manager
#   prefix    the config manager variable that is used is
#             prefix_CONFIG, the default is SDL
#
def sdl_config(env, config, prefix='SDL'):
    def parseSdlConfig(env, flags):
        config.append(prefix, **env.ParseFlags(flags))

    if config[prefix]['CONFIG']:
        value = config[prefix]['CONFIG']
        try:
            if value is True:
                env.ParseConfig('sdl-config --cflags --libs',
                                parseSdlConfig)
            else:
                env.ParseConfig(value + ' --cflags --libs',
                                parseSdlConfig)
        except:
            sys.exit('Error: %s could not be run.' % value)


# filelist()
#
# Constructs a list of files.
#
# PARAMETERS
#   pattern   a glob pattern describing the files to include
#   exclude   a list of file names to exclude
#
def filelist(pattern, exclude=[]):
    result = glob.glob(pattern)
