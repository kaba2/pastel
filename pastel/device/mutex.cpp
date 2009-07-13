#include "pastel/device/mutex.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <SDL.h>

namespace Pastel
{

	Mutex::Mutex()
		: data_((void*)SDL_CreateMutex())
	{
	}

	Mutex::~Mutex()
	{
		SDL_mutex* mutex = (SDL_mutex*)data_;
		SDL_DestroyMutex(mutex);
	}

	void Mutex::lock()
	{
		SDL_mutex* mutex = (SDL_mutex*)data_;

		int sdlError = SDL_mutexP(mutex);
		ENSURE_OP(sdlError, >=, 0);
	}

	void Mutex::unlock()
	{
		SDL_mutex* mutex = (SDL_mutex*)data_;

		int sdlError = SDL_mutexV(mutex);
		ENSURE_OP(sdlError, >=, 0);
	}

}
