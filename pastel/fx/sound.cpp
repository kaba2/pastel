#include "pastel/fx/sound.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	Sound::Sound()
		: data_()
		, sampleRate_()
	{
	}

	Sound::Sound(const Sound& that)
		: data_(that.data_)
		, sampleRate_(that.sampleRate_)
	{
	}

	Sound::~Sound()
	{
	}

	Sound& Sound::operator=(const Sound& that)
	{
		Sound copy(that);
		swap(copy);
		return *this;
	}

	void Sound::swap(Sound& that)
	{
		data_.swap(that.data_);
		std::swap(sampleRate_, that.sampleRate_);
	}

	void Sound::clear()
	{
		data_.clear();
	}

	void Sound::setSize(integer size)
	{
		if (WARNING1(size < 0, size))
		{
			return;
		}

		data_.resize(size, (float)0);
	}

	ArrayView<float> Sound::getData()
	{
		return ArrayView<float>(
			&data_.front(),
			data_.size());
	}

	ArrayView<const float> Sound::getData() const
	{
		return ArrayView<const float>(
			&data_.front(),
			data_.size());
	}

}
