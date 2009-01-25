#ifndef PASTEL_SOUND_H
#define PASTEL_SOUND_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/countedptr.h"

#include <vector>

namespace Pastel
{

	class PASTELFX Sound
		: public ReferenceCounted
	{
	public:
		Sound();
		Sound(const Sound& that);
		~Sound();

		Sound& operator=(const Sound& that);

		void swap(Sound& that);
		void clear();

		void setSize(integer size);
		void setSampleRate(integer sampleRate);

		ArrayView<float> getData();
		ArrayView<const float> getData() const;

	private:
		std::vector<float> data_;
		integer sampleRate_;
	};

	typedef CountedPtr<Sound> SoundRef;
	typedef CountedPtr<const Sound> ConstSoundRef;

}

#endif
