#include "pastel/device/timer.h"
#include "pastel/device/timedevice.h"

namespace Pastel
{

	Timer::Timer()
		: tickStart_(0)
		, ticks_(0)
	{
	}

	void Timer::setStart()
	{
		tickStart_ = timeDevice().ticks();
		ticks_ = 0;
	}

	void Timer::store()
	{
		ticks_ = timeDevice().ticks() - tickStart_;
	}

	void Timer::storeAndRestart()
	{
		const integer currentTicks = timeDevice().ticks();
		ticks_ = currentTicks - tickStart_;
		tickStart_ = currentTicks;
	}

	integer Timer::ticks() const
	{
		return ticks_;
	}

	real Timer::seconds() const
	{
		return (real)ticks_ / timeDevice().tickFrequency();
	}

}
