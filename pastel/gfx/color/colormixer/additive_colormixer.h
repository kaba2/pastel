// Description: AdditiveColorMixer class
// Detail: Adds the texture to the background
// Documentation: colormixers.txt

#ifndef PASTELGFX_ADDITIVE_COLORMIXER_H
#define PASTELGFX_ADDITIVE_COLORMIXER_H

#include "pastel/gfx/color/colormixer.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	class AdditiveColorMixer
		: public ColorMixer<Type>
	{
	public:
		virtual ~AdditiveColorMixer()
		{
		}

		virtual Type operator()(
			const Type& background,
			const Type& texture) const
		{
			return background + texture;
		}
	};

	template <typename Type>
	AdditiveColorMixer<Type> additiveColorMixer()
	{
		return AdditiveColorMixer<Type>();
	}

}

#endif
