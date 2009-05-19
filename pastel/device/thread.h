#ifndef PASTEL_THREAD_H
#define PASTEL_THREAD_H

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
		Mutex* mutex_;
		bool waitForWait_;
	};

	typedef CountedPtr<Thread> ThreadPtr;
	typedef CountedPtr<const Thread> ConstThreadPtr;

}

#endif
