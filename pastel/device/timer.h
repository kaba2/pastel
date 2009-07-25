// Description: Timer class
// Detail: Allows to measure the elapsed time between two events

#ifndef PASTEL_TIMER_H
#define PASTEL_TIMER_H

#include "pastel/device/devicelibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class PASTELDEVICE Timer
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Timer();

		void setStart();
		void store();
		void storeAndRestart();

		integer ticks() const;
		real seconds() const;

	private:
		integer tickStart_;
		integer ticks_;
	};

}

#endif
