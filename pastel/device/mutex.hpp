#ifndef PASTELDEVICE_MUTEX_HPP
#define PASTELDEVICE_MUTEX_HPP

#include "pastel/device/mutex.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline MutexLock::MutexLock(Mutex* mutex)
		: mutex_(mutex)
	{
		ENSURE(mutex_);
		mutex_->lock();
	}

	inline MutexLock::~MutexLock()
	{
		ENSURE(mutex_);
		mutex_->unlock();
	}

}

#endif
