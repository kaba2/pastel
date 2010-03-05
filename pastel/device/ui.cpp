#include "pastel/device/ui.h"
#include "pastel/device/devicesystem.h"
#include "pastel/device/timedevice.h"
#include "pastel/device/timer.h"

namespace Pastel
{

	Ui::Ui()
		: lastTime_(-1)
	{
	}

	Ui::~Ui()
	{
	}

	void Ui::handleLogic()
	{
		// Compute the time that has passed.

		const integer currentTicks =
			timeDevice().ticks();
		const real currentTime =
			(real)currentTicks /
			timeDevice().tickFrequency();

		if (lastTime_ >= 0)
		{
			const real deltaTime = 
				std::min(currentTime - lastTime_, (real)1);
			onLogic(deltaTime);
		}

		lastTime_ = currentTime;
	}

	void Ui::handleKey(bool pressed, SDLKey key)
	{
		if (pressed)
		{
			if (key == SDLK_ESCAPE)
			{
				deviceSystem().stopEventLoop();
			}
		}

		onKey(pressed, key);
	}

	// Private

	void Ui::onLogic(real deltaTime)
	{
	}

	void Ui::onKey(bool pressed, SDLKey key)
	{
	}

}
