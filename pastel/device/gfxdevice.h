// Description: GfxDevice class
// Detail: Allows the control of the graphics device

#ifndef PASTEL_GFXDEVICE_H
#define PASTEL_GFXDEVICE_H

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
		width, height > 0
		bitsPerPixel >= 0

		You must call this function before doing
		anything else with the GfxDevice.
		This will set the resolution and bit depth
		of the used video mode, as well if
		a fullscreen mode is to be used.

		width, height:
		The resolution of the video mode.

		bitsPerPixel:
		The number of bits to use for color specification.
		Setting this to zero gives the currently active
		bit depth.
				
		returns: 
		true on success, false otherwise.
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
