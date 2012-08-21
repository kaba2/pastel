// Description: AdditiveColorMixer class
// Detail: Adds the texture to the background
// Documentation: colormixers.txt

#ifndef PASTEL_ADDITIVE_COLORMIXER_H
#define PASTEL_ADDITIVE_COLORMIXER_H

#include "pastel/gfx/colormixer.h"

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
