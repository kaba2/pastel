#ifndef PASTELGFX_MIPIMAGE_TEXTURE_HPP
#define PASTELGFX_MIPIMAGE_TEXTURE_HPP

#include "pastel/gfx/texture/mipimage_texture.h"
#include "pastel/gfx/texture/linearimage_texture.h"

#include "pastel/gfx/mipmap/mipmap.h"

#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/view/repeatedconstview.h"
#include "pastel/sys/view/view_tools.h"

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
		const ArrayExtender_& extender)
		: mipMap_(&mipMap)
		, extender_(extender)
	{
	}

	template <typename Type, int N>
	Type MipImage_Texture<Type, N>::operator()(
		const Vector<dreal, N>& uv,
		const Matrix<dreal, N, N>& m) const
	{
		if (!mipMap_ || mipMap_->empty())
		{
			return Type();
		}

		integer n = m.rows();

		const Array<Type, N>& mostDetailedImage = 
			mipMap_->mostDetailed();

		// Compute detail level.

		// This is done by finding the tangent
		// vector with the greatest length and using
		// that as a isotropic sampling frequency.
		// The one of greatest length is used to ensure
		// we get rid of aliasing. 

		dreal d = 0;
		for (integer i = 0;i < n;++i)
		{
			const dreal dotMi = (m.col(i).cwiseProduct(asColumnMatrix(Vector<dreal, N>(mostDetailedImage.extent())))).squaredNorm();
			if (dotMi > d)
			{
				d = dotMi;
			}
		}

		dreal invLn2 = inverse(constantLn2<dreal>());

		const dreal level = 0.5 * std::log(d) * invLn2;

		// Handle the case where no filtering needs to be done.

		if (level <= 0)
		{
			return sampleLinear(
				evaluate(uv * Vector<dreal, N>(mostDetailedImage.extent())),
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

		integer detailLevel = std::floor(level);

		const Array<Type, N>& detailImage = 
			(*mipMap_)(detailLevel);

		Type detailSample =
			sampleLinear(
			evaluate(uv * Vector<dreal, N>(detailImage.extent())),
			detailImage, extender_);

		// Then sample from the less detailed image.
		
		integer coarseLevel = detailLevel + 1;

		const Array<Type, N>& coarseImage = 
			(*mipMap_)(coarseLevel);

		Type coarseSample =
			sampleLinear(
			evaluate(uv * Vector<dreal, N>(coarseImage.extent())),
			coarseImage, extender_);

		// Linearly interpolate these samples by the 
		// fractional detail level.

		dreal tDetail = level - detailLevel;
		return linear(detailSample, coarseSample, tDetail);
	}

}

#endif
