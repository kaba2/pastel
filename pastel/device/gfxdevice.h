/*!
\file
\brief A class controlling the graphics device.
*/

#ifndef PASTELDEVICE_GFXDEVICE_H
#define PASTELDEVICE_GFXDEVICE_H

#include "pastel/sys/mytypes.h"
#include "pastel/device/devicelibrary.h"

namespace Pastel
{


	//! A class controlling the graphics device.

	class PASTELDEVICE GfxDevice
	{
	public:
		//! Initializes the graphics device.
		/*!
		You must call this function before doing
		anything else with the GfxDevice.
		This will set the resolution and bit depth
		of the used video mode, as well as if
		a fullscreen mode is to be used.
		Returns true on success, false otherwise.
		*/
		bool initialize(
		integer width, integer height,
		integer bitsPerPixel, bool isFullScreen);

		//! Deinitializes the graphics device.
		/*!
		If you have called initialize(), you must
		call this function before ending your program.
		*/
		void deInitialize();

		//! Swaps the hardware video buffer.
		void swapBuffers();

		//! Returns the only instance of the GfxDevice.
		static GfxDevice& create();

	private:
		GfxDevice();
		~GfxDevice();

		// Prohibited
		GfxDevice(GfxDevice& that);
		// Prohibited
		GfxDevice& operator=(const GfxDevice& that);

		bool initialized_;
	};

	//! Returns the only instance of the GfxDevice.
	PASTELDEVICE GfxDevice& gfxDevice();

}

#endif
