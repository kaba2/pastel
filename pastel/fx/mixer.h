#ifndef PASTEL_MIXER_H
#define PASTEL_MIXER_H

#include "pastel/sys/mytypes.h"
#include "pastel/fx/fxlibrary.h"
#include "pastel/sys/arrayview.h"
#include "pastel/fx/machine.h"

namespace Pastel
{

	class PASTELFX Mixer
		: public Machine
	{
	public:
		Mixer();
		virtual ~Mixer();

		virtual void work(
			integer outputPort,
			const ArrayView<float>& output,
			integer updateId);

		void setPreAttenuation(real preAttenuation);
		real getPreAttenuation() const;

	private:
		real preAttenuation_;
	};

}

#endif
