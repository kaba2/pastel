#ifndef PASTEL_RIPIMAGE_TEXTURE_HPP
#define PASTEL_RIPIMAGE_TEXTURE_HPP

#include "pastel/gfx/ripimage_texture.h"
#include "pastel/gfx/linearimage_texture.h"

#include "pastel/dsp/ripmap.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Type, int N>
	RipImage_Texture<Type, N>::RipImage_Texture()
		: ripMap_(0)
		, extender_()
	{
	}

	template <typename Type, int N>
	RipImage_Texture<Type, N>::RipImage_Texture(
		const RipMap<Type, N>& ripMap,
		const ArrayExtender<N, Type>& extender)
		: ripMap_(&ripMap)
		, extender_(extender)
	{
	}

	template <typename Type, int N>
	Type RipImage_Texture<Type, N>::operator()(
		const Vector<real, N>& uv,
		const Matrix<real, N, N>& m) const
	{
		if (!ripMap_ || ripMap_->empty())
		{
			return Type();
		}

		const Array<Type, N>& mostDetailedImage = 
			ripMap_->mostDetailed();

		const Vector<real, N> radius = 
			max(abs(m)) * Vector<real, N>(mostDetailedImage.extent());

		if (allLessEqual(radius, 1))
		{
			// Magnification: just do linear interpolation.

			return sampleLinear(
				evaluate(uv * Vector<real, N>(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		const real invLn2 = inverse(constantLn2<real>());
		const Vector<real, N> level(max(evaluate(log(radius) * invLn2), 0));

		if (allLessEqual(level, 0))
		{
			return sampleLinear(
				evaluate(uv * Vector<real, N>(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		if (anyGreaterEqual(level, Vector<real, N>(ripMap_->levels() - 1)))
		{
			// Return the coarsest ripmap pixel.

			return ripMap_->coarsest()(0);
		}

		// Gather samples from all of the neighboring 
		// 2^n ripmaps.

		Vector<integer, N> p(floor(level));
		const Vector<real, N> tDetail = level - Vector<real, N>(p);

		const integer n = p.size();
		const integer samples = 1 << n;

		Tuple<Type, ModifyN<N, 1 << N>::Result> valueSet(ofDimension(samples));
		Tuple<bool, N> s(ofDimension(n), false);
		for (integer i = 0;i < samples;++i)
		{
			const Array<Type, N>& image = (*ripMap_)(p);

			valueSet[i] = sampleLinear(
				evaluate(uv * Vector<real, N>(image.extent())),
				image, extender_);

			integer axis = 0;
			while(axis < n)
			{
				if (s[axis])
				{
					s[axis] = false;
					--p[axis];
				}
				else
				{
					s[axis] = true;
					++p[axis];
					break;
				}
				++axis;
			}
		}

		// Linearly interpolate between those samples.

		return linear(tDetail, 
			randomAccessRange(valueSet.begin(), valueSet.end()));
	}

}

#endif
