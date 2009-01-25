#include "pastel/fx/sinsynth.h"
#include "pastel/numeric/constant.h"

#include <cmath>

namespace Pastel
{

	SinSynth::SinSynth()
		: Instrument()
		, phase_(0)
	{
		setOutputPorts(1);
	}

	SinSynth::~SinSynth()
	{
	}

	void SinSynth::setPhase(real phase)
	{
		phase_ = phase;
	}

	void SinSynth::work(
		integer outputPort,
		const ArrayView<float>& output,
		integer updateId)
	{
		if (outputPort == 0)
		{
			integer outputSize = output.size();

			real angularFrequency = 2 * constantPi<real>() *
				getFrequency() / sampleRate();

			real currentPhase = phase_;
			for (integer i = 0;i < outputSize;++i)
			{
				output[i] =
					std::pow(
					std::sin(currentPhase), 5);
				currentPhase += angularFrequency;
			}

			phase_ += angularFrequency * outputSize;
		}
	}

}
