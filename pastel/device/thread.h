#ifndef PASTELDEVICE_THREAD_H
#define PASTELDEVICE_THREAD_H

#include "pastel/device/mutex.h"
#include "pastel/sys/countedptr.h"

#include <vector>

#include <SDL.h>

namespace Pastel
{

	class PASTELDEVICE Thread
		: public ReferenceCounted
	{
	public:
		Thread();
		virtual ~Thread();

		void launch();
		void wait();
		bool running() const;

	private:
		virtual void run() = 0;

		static int launchHelper(void* data);

		SDL_Thread* volatile thread_;
		mutable Mutex mutex_;
	};

	typedef CountedPtr<Thread> ThreadPtr;
	typedef CountedPtr<const Thread> ConstThreadPtr;

}

#endif
