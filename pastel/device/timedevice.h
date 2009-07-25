// Description: TimeDevice class
// Detail: Allows controlling timing related stuff

#ifndef PASTEL_TIMEDEVICE_H
#define PASTEL_TIMEDEVICE_H

#include "pastel/sys/mytypes.h"
#include "pastel/device/devicelibrary.h"
#include "pastel/sys/memoryview.h"

namespace Pastel
{

	//! A class for controlling the timing device.
	class PASTELDEVICE TimeDevice
	{
	public:
		//! Returns the number of ticks thus far.
		/*!
		The timer keeps a running count of 'ticks'.
		A tick is a time duration equal to the inverse
		of tickFrequency().
		This can be used to measure the time spent
		by a procedure by getting the ticks just before
		and right after the procedure and dividing by
		the tick frequency.
		*/
		integer ticks() const;

		//! Returns the frequency of the ticks.
		integer tickFrequency() const;

		//! Waits for the given amount of milliseconds.
		void delay(integer milliSeconds);

		//! Returns the only instance of TimeDevice.
		static TimeDevice& create();

	private:
		TimeDevice();
		~TimeDevice();

		// Prohibited
		TimeDevice(TimeDevice& that);
		// Prohibited
		TimeDevice& operator=(const TimeDevice& that);
	};

	//! Returns the only instance of TimeDevice.
	PASTELDEVICE TimeDevice& timeDevice();

}

#endif
