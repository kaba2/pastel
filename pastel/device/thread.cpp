#include "pastel/device/thread.h"

#include <SDL.h>

namespace Pastel
{

	Thread::Thread()
		: thread_(0)
		, mutex_()
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::launch()
	{
		{
			MutexLock lock(&mutex_);
			if (REPORT(running()))
			{
				return;
			}
			thread_ = SDL_CreateThread(launchHelper, (void*)this);
		}
	}

	void Thread::wait()
	{
		if (thread_)
		{
			SDL_WaitThread(thread_, 0);
		}
	}

	bool Thread::running() const
	{
		bool result = false;
		{
			MutexLock lock(&mutex_);
			result = (thread_ != 0);
		}
		return result;
	}

	// Private

	int Thread::launchHelper(void* data)
	{
		Thread* thread = (Thread*)data;
		
		thread->run();

		{
			MutexLock lock(&thread->mutex_);
			thread->thread_ = 0;
		}

		return 0;
	}


}
