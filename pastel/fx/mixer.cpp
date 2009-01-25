#include "pastel/fx/mixer.h"

namespace Pastel
{

	Mixer::Mixer()
		: Machine()
		, preAttenuation_(0.05)
	{
		setInputPorts(8);
		setOutputPorts(1);
	}

	Mixer::~Mixer()
	{
	}

	void Mixer::work(
		integer outputPort,
		const ArrayView<float>& output,
		integer updateId)
	{
		if (outputPort == 0)
		{
			ArrayView<float> input;
			integer size = 0;
			integer inputs = inputPorts();
			integer outputSize = output.size();

			// Zero the output buffer
			for (integer x = 0;x < outputSize;++x)
			{
				output[x] = 0;
			}

			// Accumulate the inputs into
			// one buffer
			for (integer i = 0;i < inputs;++i)
			{
				input = getInput(i, updateId);

				size = std::min(
					input.size(), outputSize);

				for (integer x = 0;x < size;++x)
				{
					output[x] += input[x] * preAttenuation_;
				}
			}
		}
	}

	void Mixer::setPreAttenuation(real preAttenuation)
	{
		if (WARNING1(preAttenuation < 0, preAttenuation))
		{
			return;
		}

		preAttenuation_ = preAttenuation;
	}

	real Mixer::getPreAttenuation() const
	{
		return preAttenuation_;
	}

}
