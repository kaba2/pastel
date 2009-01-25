#ifndef PASTELGFX_TRANSPARENTCOLORMIXER_H
#define PASTELGFX_TRANSPARENTCOLORMIXER_H

#include "pastel/gfx/colormixer.h"

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
