#ifndef PASTEL_SINSYNTH_H
#define PASTEL_SINSYNTH_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/fx/instrument.h"
#include "pastel/sys/arrayview.h"

namespace Pastel
{

	class PASTELFX SinSynth
		: public Instrument
	{
	public:
		SinSynth();
		virtual ~SinSynth();

		void setPhase(real phase);
		real getPhase() const;

		virtual void work(
			integer outputPort,
			const ArrayView<float>& output,
			integer updateId);

	private:
		real phase_;
	};

}

#endif
