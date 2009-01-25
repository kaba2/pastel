#ifndef PASTEL_RESAMPLER_H
#define PASTEL_RESAMPLER_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/fx/instrument.h"

namespace Pastel
{

	class PASTELFX Resampler
		: public Instrument
	{
	public:
		virtual ~Resampler();

		virtual void work(
			integer outputPort,
			const ArrayView<float>& output,
			integer updateId);

	private:
	};

}

#endif
