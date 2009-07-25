// Description: AssignColorMixer
// Detail: Replaces the background with the texture

#ifndef PASTEL_ASSIGNCOLORMIXER_H
#define PASTEL_ASSIGNCOLORMIXER_H

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
