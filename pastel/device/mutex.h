// Description: Mutex class
// Detail: Mutual exclusion in a concurrent program

#ifndef PASTEL_MUTEX_H
#define PASTEL_MUTEX_H

#include "pastel/sys/mytypes.h"
#include "pastel/device/devicelibrary.h"

namespace Pastel
{

	// A class for mutual exclusion in a concurrent program.
	class PASTELDEVICE Mutex
	{
	public:
		Mutex();
		~Mutex();

		//! Locks the mutex.
		void lock();

		//! Unlocks the mutex.
		void unlock();

	private:
		// Prohibited
		Mutex(const Mutex& that);
		// Prohibited
		Mutex& operator=(const Mutex& that);

		// The following should actually read
		// SDL_mutex* data_;
		// This is a dirty trick,
		// but I want to keep SDL purely as an
		// implementation detail.

		void* data_;
	};

	//! A class for automating mutex locking behaviour in a scope.
	/*!
	The MutexLock class locks the mutex given to its constructor.
	The mutex is unlocked when the MutexLock is destructed.
	*/
	class PASTELDEVICE MutexLock
	{
	public:
		//! Constructs with the given mutex.
		explicit MutexLock(Mutex* mutex);
		~MutexLock();

	private:
		// Prohibited
		MutexLock();
		// Prohibited
		MutexLock(const MutexLock& that);
		// Prohibited
		MutexLock& operator=(const MutexLock& that);

		Mutex* mutex_;
	};

}

#include "pastel/device/mutex.hpp"

#endif
