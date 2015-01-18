// Description: AssignColorMixer
// Detail: Replaces the background with the texture
// Documentation: colormixers.txt

#ifndef PASTELGFX_ASSIGN_COLORMIXER_H
#define PASTELGFX_ASSIGN_COLORMIXER_H

#include "pastel/gfx/colormixer.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	class AssignColorMixer
		: public ColorMixer<Type>
	{
	public:
		virtual ~AssignColorMixer()
		{
		}

		virtual Type operator()(
			const Type& background,
			const Type& texture) const
		{
			return texture;
		}
	};

	template <typename Type>
	AssignColorMixer<Type> assignColorMixer()
	{
		return AssignColorMixer<Type>();
	}

}

#endif
