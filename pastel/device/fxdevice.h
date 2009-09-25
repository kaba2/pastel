// Description: FxDevice class
// Detail: Allows the control of the audio device
// Documentation: sound.txt

#ifndef PASTEL_FXDEVICE_H
#define PASTEL_FXDEVICE_H

#include "pastel/sys/mytypes.h"
#include "pastel/device/devicelibrary.h"
#include "pastel/sys/memoryview.h"

namespace Pastel
{

	//! A class for controlling the audio device.

	class PASTELDEVICE FxDevice
	{
	public:
		class SoundFormat
		{
		public:
			enum Enum
			{
				None,
				Int8,
				UInt8,
				Int16LittleEndian,
				UInt16LittleEndian,
				Int16BigEndian,
				UInt16BigEndian
			};
		};

		typedef void (MixerCallBack)(
			const MemoryView<int8>& data);

		//! Initializes the audio device.
		/*!
		You must call this function before doing
		anything with the FxDevice. This is used
		to set the sampling rate, the number of
		channels, the sample bitcount, the
		buffer size and the streaming callback.
		*/
		bool initialize(
			integer sampleRate,
			integer channels,
			SoundFormat::Enum format,
			integer bufferSamples,
			MixerCallBack* callBack);

		//! Deinitializes the audio device
		/*!
		You must call this function at the
		end of the use of the audio device.
		*/
		void deInitialize();

		//! Starts calling the mixer callback.
		void play();

		//! Stops calling the mixer callback.
		void stop();

		//! Returns the only instance of a FxDevice.
		static FxDevice& create();

	private:
		FxDevice();
		~FxDevice();

		// Prohibited
		FxDevice(FxDevice& that);
		// Prohibited
		FxDevice& operator=(const FxDevice& that);

		bool playing_;
		bool initialized_;
	};

	//! Returns the only instance of a FxDevice.
	PASTELDEVICE FxDevice& fxDevice();

}

#endif
