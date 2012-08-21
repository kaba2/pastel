// Description: An abstract user-interface

#ifndef PASTEL_UI_H
#define PASTEL_UI_H

#include "pastel/device/devicelibrary.h"

#include "pastel/sys/mytypes.h"

#include <SDL.h>

namespace Pastel
{

	class PASTELDEVICE Ui
	{
	public:
		Ui();
		virtual ~Ui();

		void handleLogic();
		void handleKey(bool pressed, SDLKey key);
		
	private:
		virtual void onLogic(real deltaTime);
		virtual void onKey(bool pressed, SDLKey key);

		real lastTime_;
	};

}

#endif
