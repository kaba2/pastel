#include "pastel/fx/instrument.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	Instrument::Instrument()
		: channels_()
		, indexAllocator_()
	{
	}

	Instrument::~Instrument()
	{
	}

	integer Instrument::allocateChannel()
	{
		const integer MAX_CHANNELS = 256;

		if (WARNING2(indexAllocator_.size() >= MAX_CHANNELS,
			indexAllocator_.size(), MAX_CHANNELS))
		{
			return -1;
		}

		return indexAllocator_.allocate();
	}

	void Instrument::deAllocateChannel(integer channel)
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return;
		}

		indexAllocator_.deAllocate(channel);
	}

	void Instrument::setFrequency(integer channel, real frequency)
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return;
		}

		channels_[channel].frequency_ = frequency;
	}

	real Instrument::getFrequency(integer channel) const
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return 0;
		}

		return channels_[channel].frequency_;
	}

	void Instrument::play(integer channel)
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return;
		}

		channels_[channel].isPlaying_ = true;
	}

	void Instrument::stop(integer channel)
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return;
		}

		channels_[channel].isPlaying_ = false;
	}

	bool Instrument::isPlaying(integer channel) const
	{
		if (WARNING1(channel < 0, channel) ||
			WARNING2(channel >= channels_.size(),
			channel, channels_.size()))
		{
			return false;
		}

		return channels_[channel].isPlaying_;
	}

}
