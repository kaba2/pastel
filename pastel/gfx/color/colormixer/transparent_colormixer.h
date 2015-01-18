// Description: TransparentColorMixer
// Detail: Linearly interpolates between background and texture
// Documentation: colormixers.txt

#ifndef PASTELGFX_TRANSPARENT_COLORMIXER_H
#define PASTELGFX_TRANSPARENT_COLORMIXER_H

#include "pastel/gfx/color/colormixer.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	class TransparentColorMixer
		: public ColorMixer<Type>
	{
	public:
		TransparentColorMixer()
			: opaqueness_(1)
		{
		}

		explicit TransparentColorMixer(
			real opaqueness)
			: opaqueness_(opaqueness)
		{
		}

		virtual ~TransparentColorMixer()
		{
		}

		virtual Type operator()(
			const Type& background,
			const Type& texture) const
		{
			return (1 - opaqueness_) * background +
				opaqueness_ * texture;
		}

		void setOpaqueness(real opaqueness)
		{
			opaqueness_ = opaqueness;
		}

		real opaqueness() const
		{
			return opaqueness_;
		}

	private:
		real opaqueness_;
	};

	template <typename Type>
	TransparentColorMixer<Type> transparentColorMixer(real opaqueness)
	{
		return TransparentColorMixer<Type>(opaqueness);
	}

}

#endif
