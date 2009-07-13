#include "pastel/device/gfxdevice.h"
#include "pastel/device/devicesystem.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <GL/glew.h>
#include <SDL.h>

namespace Pastel
{

	// GfxDevice

	bool GfxDevice::initialize(
		integer width, integer height,
		integer bitsPerPixel, bool isFullScreen)
	{
		log() << "Initializing SDL video..." << logNewLine;

		ENSURE_OP(width, >, 0);
		ENSURE_OP(height, >, 0);
		ENSURE_OP(bitsPerPixel, >=, 0);
		ENSURE(!initialized_);
		ENSURE(deviceSystem().initialize());

		int sdlError = 0;

		sdlError = SDL_InitSubSystem(SDL_INIT_VIDEO);
		ENSURE_OP(sdlError, >=, 0);

		// Initialize SDL's OpenGl device

		sdlError = SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		ENSURE_OP(sdlError, >=, 0);

		sdlError = SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		ENSURE_OP(sdlError, >=, 0);

		sdlError = SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
		ENSURE_OP(sdlError, >=, 0);

		sdlError = SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
		ENSURE_OP(sdlError, >=, 0);

		sdlError = SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		ENSURE_OP(sdlError, >=, 0);

		int flags = SDL_OPENGL;

		if (isFullScreen)
		{
			flags |= SDL_FULLSCREEN;
		}

		SDL_Surface* videoSurface = SDL_SetVideoMode(
			width, height, bitsPerPixel, flags);
		ENSURE(videoSurface);

		// Initialize Glew

		log() << "Initializing Glew..." << logNewLine;

		GLenum glewError = glewInit();
		ENSURE_OP(glewError, ==, GLEW_OK);

		log() << "GfxDevice: non-power-of-two textures ";
		if (GLEW_ARB_texture_non_power_of_two)
		{
			log() << "supported." << logNewLine;
		}
		else
		{
			log() << "not supported." << logNewLine;
		}

		initialized_ = true;

		return true;
	}

	void GfxDevice::deInitialize()
	{
		log() << "Deinitializing SDL video..." << logNewLine;

		ENSURE(initialized_);

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		initialized_ = false;

		deviceSystem().deInitialize();
	}

	void GfxDevice::swapBuffers()
	{
		ENSURE(initialized_);

		SDL_GL_SwapBuffers( );
	}

	GfxDevice& GfxDevice::create()
	{
		static GfxDevice theGfxDevice__;
		return theGfxDevice__;
	}

	// Private

	GfxDevice::GfxDevice()
		: initialized_(false)
	{
	}

	GfxDevice::~GfxDevice()
	{
		if (initialized_)
		{
			deInitialize();
		}
	}

	// Other GfxDevice interface

	PASTELDEVICE GfxDevice& gfxDevice()
	{
		return GfxDevice::create();
	}

}
