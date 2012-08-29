// Description: FunctorColorMixer class
// Detail: Combines background and texture using the supplied functor
// Documentation: colormixers.txt

#ifndef PASTELGFX_FUNCTOR_COLORMIXER_H
#define PASTELGFX_FUNCTOR_COLORMIXER_H

#include "pastel/gfx/colormixer.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type, typename Functor>
	class FunctorColorMixer
		: public ColorMixer<Type>
	{
	public:
		explicit FunctorColorMixer(
			const Functor& functor)
			: functor_(functor)
		{
		}

		virtual ~FunctorColorMixer()
		{
		}

		virtual Type operator()(
			const Type& background,
			const Type& texture) const
		{
			return functor_(background, texture);
		}

	private:
		const Functor functor_;
	};

	template <typename Type, typename Functor>
	FunctorColorMixer<Type, Functor> functorColorMixer(const Functor& functor)
	{
		return FunctorColorMixer<Type, Functor>(functor);
	}

}

#endif
