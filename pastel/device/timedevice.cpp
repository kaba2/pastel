#include "pastel/device/timedevice.h"
#include "pastel/device/devicesystem.h"
#include "pastel/sys/ensure.h"

#include <SDL.h>

namespace Pastel
{

	// TimeDevice

	integer TimeDevice::ticks() const
	{
		return (integer)SDL_GetTicks();
	}

	integer TimeDevice::tickFrequency() const
	{
		return (integer)1000;
	}

	void TimeDevice::delay(integer milliSeconds)
	{
		ENSURE_OP(milliSeconds, >=, 0);

		SDL_Delay((Uint32)milliSeconds);
	}

	TimeDevice& TimeDevice::create()
	{
		static TimeDevice theTimeDevice__;
		return theTimeDevice__;
	}

	// Private

	TimeDevice::TimeDevice()
	{
	}

	TimeDevice::~TimeDevice()
	{
	}

	// Other TimeDevice interface

	PASTELDEVICE TimeDevice& timeDevice()
	{
		return TimeDevice::create();
	}

}
