#ifndef PASTELGFX_FILTERDESIGN_H
#define PASTELGFX_FILTERDESIGN_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	PASTELGFX integer designFilter(float filterFactor, float transitionWidth);

	PASTELGFX integer designBox(float transitionWidth);
	PASTELGFX integer designBartlett(float transitionWidth);
	PASTELGFX integer designHanning(float transitionWidth);
	PASTELGFX integer designHamming(float transitionWidth);
	PASTELGFX integer designBlackman(float transitionWidth);

}

#endif
