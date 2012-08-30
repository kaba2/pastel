#ifndef PASTELGFX_MIPIMAGE_TEXTURE_HPP
#define PASTELGFX_MIPIMAGE_TEXTURE_HPP

#include "pastel/gfx/mipimage_texture.h"
#include "pastel/gfx/linearimage_texture.h"

#include "pastel/gfx/mipmap.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/repeatedconstview.h"
#include "pastel/sys/view_tools.h"

namespace Pastel
{

	template <typename Type, int N>
	MipImage_Texture<Type, N>::MipImage_Texture()
		: mipMap_(0)
		, extender_()
	{
	}

	template <typename Type, int N>
	MipImage_Texture<Type, N>::MipImage_Texture(
		const MipMap<Type, N>& mipMap,
		const ArrayExtender<N, Type>& extender)
		: mipMap_(&mipMap)
		, extender_(extender)
	{
	}

	template <typename Type, int N>
	Type MipImage_Texture<Type, N>::operator()(
		const Vector<real, N>& uv,
		const Matrix<real, N, N>& m) const
	{
		if (!mipMap_ || mipMap_->empty())
		{
			return Type();
		}

		const integer n = m.height();
		const Array<Type, N>& mostDetailedImage = 
			mipMap_->mostDetailed();

		// Compute detail level.

		// This is done by finding the tangent
		// vector with the greatest length and using
		// that as a isotropic sampling frequency.
		// The one of greatest length is used to ensure
		// we get rid of aliasing. 

		real d = 0;
		for (integer i = 0;i < n;++i)
		{
			const real dotMi = dot(m[i] * Vector<real, N>(mostDetailedImage.extent()));
			if (dotMi > d)
			{
				d = dotMi;
			}
		}

		const real invLn2 = inverse(constantLn2<real>());
		const real level = 0.5 * std::log(d) * invLn2;

		// Handle the case where no filtering needs to be done.

		if (level <= 0)
		{
			return sampleLinear(
				evaluate(uv * Vector<real, N>(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		// Handle the case where the image is smaller than
		// a single pixel.

		if (level >= mipMap_->levels() - 1)
		{
			return mipMap_->coarsest()(0);
		}

		// Gather samples from the 2 neighboring mipmaps.

		// First sample from the more detailed image.

		const integer detailLevel = std::floor(level);
		const Array<Type, N>& detailImage = 
			(*mipMap_)(detailLevel);

		const Type detailSample =
			sampleLinear(
			evaluate(uv * Vector<real, N>(detailImage.extent())),
			detailImage, extender_);

		// Then sample from the less detailed image.
		
		const integer coarseLevel = detailLevel + 1;
		const Array<Type, N>& coarseImage = 
			(*mipMap_)(coarseLevel);

		const Type coarseSample =
			sampleLinear(
			evaluate(uv * Vector<real, N>(coarseImage.extent())),
			coarseImage, extender_);

		// Linearly interpolate these samples by the 
		// fractional detail level.

		const real tDetail = level - detailLevel;
		return linear(detailSample, coarseSample, tDetail);
	}

}

#endif
