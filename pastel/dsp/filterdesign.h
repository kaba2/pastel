#ifndef PASTELDSP_FILTERDESIGN_H
#define PASTELDSP_FILTERDESIGN_H

#include "pastel/dsp/dsplibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	PASTELDSP integer designFilter(float filterFactor, float transitionWidth);

	PASTELDSP integer designBox(float transitionWidth);
	PASTELDSP integer designBartlett(float transitionWidth);
	PASTELDSP integer designHanning(float transitionWidth);
	PASTELDSP integer designHamming(float transitionWidth);
	PASTELDSP integer designBlackman(float transitionWidth);

}

#endif
