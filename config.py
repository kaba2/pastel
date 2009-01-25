###
### Configuration options for Pastel
###

## GENERAL INFO
#
# The default configuration is suitable for building in Unix
# system. It should autodetect everything if libraries have been
# installed in standard locations.
#
# *_CPPPATH variables should be strings containing a space separated
# list of paths. These paths are searched for include files.
#
# *_LIBPATH are like *_CPPPATH but these paths are searched for
# libraries.
#
# *_LIBS should be strings containing space separated list of library
# names, NOT including the file prefix ('lib' on Unix) or the file
# suffix (e.g. '.a', '.so' on Unix, '.lib' on Windows)
#
# *_CXXFLAGS are strings of C++ compiler flags. They are passed to the
# compiler as-is.
#
# *_LINKFLAGS are like *_CXXFLAGS but they are passed to linker.
#

## SDL options

# If SDL_CONFIG is set to True, use sdl-config to detect compiler
# flags for SDL.  If set to a string, the string should be a path to
# the sdl-config program.  If set to False, use SDL_* values.
SDL_CONFIG = True

SDL_CPPPATH = None
SDL_LIBPATH = None
SDL_CXXFLAGS = None
SDL_LINKFLAGS = None
SDL_LIBS = None


## RPly options
RPly_CPPPATH = None
RPly_LIBPATH = None
RPly_LIBS = 'rply'


## OpenGL options
OpenGL_CPPPATH = None
OpenGL_LIBPATH = None

# Usually 'GL GLU' on Unix, 'opengl32 glu32' on Windows.
OpenGL_LIBS = 'GL GLU'


## GLEW options
GLEW_CPPPATH = None
GLEW_LIBPATH = None

# Usually 'GLEW' on Unix, 'glew32' on Windows.
GLEW_LIBS = 'GLEW'


## Boost options
Boost_CPPPATH = None
Boost_LIBPATH = None
