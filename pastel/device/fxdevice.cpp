#include "pastel/device/fxdevice.h"
#include "pastel/device/devicesystem.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"

#include <SDL.h>

namespace Pastel
{

	namespace
	{

		FxDevice::MixerCallBack* mixerCallBack__ = 0;

		void sdlMixerCallBack(
			void* userData, Uint8* stream,
			int length);

		Uint16 soundFormatToSdlFormat(
			FxDevice::SoundFormat::Enum soundFormat);

		void sdlMixerCallBack(
			void* userData, Uint8* stream,
			int length)
		{
			unused(userData);
			if (mixerCallBack__)
			{
				(*mixerCallBack__)(MemoryView<int8>(
					(int8*)stream, (integer)length));
			}
		}

		Uint16 soundFormatToSdlFormat(
			FxDevice::SoundFormat::Enum soundFormat)
		{
			switch(soundFormat)
			{
			case FxDevice::SoundFormat::Int8:
				return AUDIO_S8;
				break;
			case FxDevice::SoundFormat::UInt8:
				return AUDIO_U8;
				break;
			case FxDevice::SoundFormat::Int16LittleEndian:
				return AUDIO_S16LSB;
				break;
			case FxDevice::SoundFormat::UInt16LittleEndian:
				return AUDIO_U16LSB;
				break;
			case FxDevice::SoundFormat::Int16BigEndian:
				return AUDIO_S16MSB;
				break;
			case FxDevice::SoundFormat::UInt16BigEndian:
				return AUDIO_U16MSB;
				break;
			case FxDevice::SoundFormat::None:
				ENSURE(soundFormat != FxDevice::SoundFormat::None);
				return 0;
			};

			return 0;
		}

	}

	// FxDevice

	bool FxDevice::initialize(
		integer sampleRate,
		integer channels,
		SoundFormat::Enum format,
		integer bufferSamples,
		MixerCallBack* callBack)
	{
		//const integer DefaultBufferSamples = 1024;
		//const integer DefaultSampleRate = 44100;
		//const integer DefaultChannels = 1;

		ENSURE(callBack);
		ENSURE_OP(bufferSamples, >, 0);
		ENSURE_OP(sampleRate, >, 0);
		ENSURE1(channels == 1 || channels == 2, channels);
		ENSURE(!initialized_);
		ENSURE(deviceSystem().initialize());

		int sdlError = 0;

		sdlError = SDL_InitSubSystem(SDL_INIT_AUDIO);

		ENSURE_OP(sdlError, >=, 0);

		SDL_AudioSpec audioSpec;
		audioSpec.freq = sampleRate;
		audioSpec.format = soundFormatToSdlFormat(format);
		audioSpec.channels = channels;
		audioSpec.silence = 0;
		audioSpec.samples = bufferSamples;
		audioSpec.size = 0;
		audioSpec.callback = sdlMixerCallBack;
		audioSpec.userdata = 0;

		mixerCallBack__ = callBack;

		sdlError = SDL_OpenAudio(&audioSpec, 0);

		ENSURE_OP(sdlError, >=, 0);

		initialized_ = true;

		return true;
	}

	void FxDevice::deInitialize()
	{
		ENSURE(initialized_);

		SDL_CloseAudio();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		initialized_ = false;

		deviceSystem().deInitialize();
	}

	void FxDevice::play()
	{
		ENSURE(initialized_);
		ENSURE(!playing_);

		SDL_PauseAudio(0);

		playing_ = true;
	}

	void FxDevice::stop()
	{
		ENSURE(initialized_);
		ENSURE(playing_);

		SDL_PauseAudio(1);

		playing_ = false;
	}

	FxDevice& FxDevice::create()
	{
		static FxDevice theFxDevice__;
		return theFxDevice__;
	}

	// Private

	FxDevice::FxDevice()
		: playing_(false)
		, initialized_(false)
	{
	}

	FxDevice::~FxDevice()
	{
		if (initialized_)
		{
			if (playing_)
			{
				stop();
			}

			deInitialize();
		}
	}

	// Other FxDevice interface

	PASTELDEVICE FxDevice& fxDevice()
	{
		return FxDevice::create();
	}

}
