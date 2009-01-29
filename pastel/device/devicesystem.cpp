#include "pastel/device/devicesystem.h"
#include "pastel/device/timedevice.h"
#include "pastel/device/timer.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <SDL.h>

namespace Pastel
{

	// DeviceSystem

	bool DeviceSystem::initialize()
	{
		if (referenceCount_ == 0)
		{
			log() << "Initializing SDL..." << logNewLine;

			if (SDL_Init(0) < 0)
			{
				return false;
			}
		}

		++referenceCount_;

		return true;
	}

	void DeviceSystem::deInitialize()
	{
		--referenceCount_;
		if (referenceCount_ == 0)
		{
			log() << "Deinitializing SDL..." << logNewLine;
			SDL_Quit();
		}
	}

	bool DeviceSystem::keyDown(SDLKey key) const
	{
		return keyDown_[key % KeyTableSize];
	}

	void DeviceSystem::setCaption(const std::string& text)
	{
		SDL_WM_SetCaption(text.c_str(), 0);
	}

	std::string DeviceSystem::getCaption() const
	{
		char* title = 0;
		char* icon = 0;

		SDL_WM_GetCaption(&title, &icon);

		return std::string(title);
	}

	void DeviceSystem::setKeyHandler(
		KeyHandler* keyHandler)
	{
		keyHandler_ = keyHandler;
	}

	void DeviceSystem::setLogicHandler(
		LogicHandler* logicHandler)
	{
		logicHandler_ = logicHandler;
	}

	void DeviceSystem::startEventLoop(integer fps)
	{
		const real frameDelay = (real)1 / fps;
		Timer timer;

		integer logicTimer = 0;

		initialize();

		try
		{
			eventLoopActive_ = true;

			timer.setStart();
			while (eventLoopActive_)
			{
				SDL_Event event;

				while ( SDL_PollEvent( &event ) )
				{
					switch( event.type )
					{
					case SDL_KEYDOWN:
						{
							if (keyHandler_)
							{
								SDL_keysym* keysym = &event.key.keysym;
								keyHandler_(true, keysym->sym);
								keyDown_[keysym->sym % KeyTableSize] = true;
							}
						}
						break;
					case SDL_KEYUP:
						{
							if (keyHandler_)
							{
								SDL_keysym* keysym = &event.key.keysym;
								keyHandler_(false, keysym->sym);
								keyDown_[keysym->sym % KeyTableSize] = false;
							}
						}
						break;
					case SDL_QUIT:
						{
							eventLoopActive_ = true;
						}
						break;
					}
				}

				timer.store();

				if (logicHandler_ && timer.seconds() >= frameDelay)
				{
					timer.setStart();
					logicHandler_();
					timer.store();

					cpuUsage_ = (real)100 * (timer.seconds() / frameDelay);
				}

				// There are some issues with this to consider.
				// The SDL timer has a low resolution of 1000hz.
				// Consider this loop runs 200hz. Then the tick count
				// is at most 5 and there are only 6 different
				// levels available to describe the time taken
				// by the logic.
			}
		}
		catch(...)
		{
			deInitialize();
			throw;
		}

		deInitialize();
	}

	void DeviceSystem::stopEventLoop()
	{
		eventLoopActive_ = false;
	}

	Integer2 DeviceSystem::mouse(bool* leftButton, bool* rightButton)
	{
		int x = 0;
		int y = 0;
		Uint8 buttons = SDL_GetMouseState(&x, &y);

		if (leftButton)
		{
			*leftButton = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
		}
		if (rightButton)
		{
			*rightButton = (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		}

		return Integer2(x, y);
	}

	real DeviceSystem::cpuUsage() const
	{
		return cpuUsage_;
	}

	DeviceSystem& DeviceSystem::create()
	{
		static DeviceSystem theDeviceSystem__;
		return theDeviceSystem__;
	}

	// Other DeviceSystem interface

	PASTELDEVICE DeviceSystem& deviceSystem()
	{
		return DeviceSystem::create();
	}

	// Private

	DeviceSystem::DeviceSystem()
		: referenceCount_(0)
		, eventLoopActive_(false)
		, keyHandler_()
		, logicHandler_()
		, cpuUsage_(0)
		, keyDown_(KeyTableSize, false)
	{
	}

	DeviceSystem::~DeviceSystem()
	{
	}

}
