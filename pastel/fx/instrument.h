#ifndef PASTEL_INSTRUMENT_H
#define PASTEL_INSTRUMENT_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/fx/machine.h"
#include "pastel/sys/indexallocator.h"

#include <vector>

namespace Pastel
{

	class PASTELFX Instrument
		: public Machine
	{
	public:
		Instrument();
		virtual ~Instrument();

		integer allocateChannel();
		void deAllocateChannel(integer channel);

		void setFrequency(integer channel, real frequency);
		real getFrequency(integer channel) const;

		void play(integer channel);
		void stop(integer channel);

		bool isPlaying(integer channel) const;

	private:
		struct Channel
		{
			real frequency_;
			bool isPlaying_;
		};

		std::vector<Channel> channels_;
		IndexAllocator indexAllocator_;
	};

}

#endif
