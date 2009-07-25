// Description: Devices and event loop

#ifndef PASTEL_DEVICESYSTEM_H
#define PASTEL_DEVICESYSTEM_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/device/devicelibrary.h"

#include <SDL.h>

#include <memory>
#include <string>
#include <vector>

namespace Pastel
{
	typedef void KeyHandler(bool, SDLKey);
	typedef void LogicHandler();

	//! A class for device and event handling.
	class PASTELDEVICE DeviceSystem
	{
	public:
		//! Initializes the device system.
		/*!
		This function must be called before
		initializing any devices.
		*/
		bool initialize();

		//! Deinitializes the device system.
		/*!
		If initialize() has been called, then
		this function must be called before the
		end of the program.
		*/
		void deInitialize();

		bool keyDown(SDLKey key) const;

		void setCaption(const std::string& text);
		std::string getCaption() const;

		void setKeyHandler(KeyHandler* keyHandler);
		void setLogicHandler(LogicHandler* logicHandler);

		//! Starts running the event loop.
		void startEventLoop(integer fps = 100);

		//! Stops the event loop.
		void stopEventLoop();

		//! Retrieves the mouse state.
		Integer2 mouse(bool* leftButton = 0, bool* rightButton = 0);

		//! Returns the cpu usage of the program in percents.
		real cpuUsage() const;

		//! Returns the only instance of DeviceSystem.
		static DeviceSystem& create();

	private:
		DeviceSystem();
		~DeviceSystem();

		// Prohibited
		DeviceSystem(DeviceSystem& that);
		// Prohibited
		DeviceSystem& operator=(const DeviceSystem& that);

		integer referenceCount_;
		bool eventLoopActive_;
		KeyHandler* keyHandler_;
		LogicHandler* logicHandler_;
		real cpuUsage_;

		enum
		{
			KeyTableSize = 1024
		};

		std::vector<bool> keyDown_;
	};

	//! Returns the only instance of DeviceSystem.
	PASTELDEVICE DeviceSystem& deviceSystem();

}

#endif
