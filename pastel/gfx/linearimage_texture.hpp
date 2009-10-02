#ifndef PASTEL_LINEARIMAGE_TEXTURE_HPP
#define PASTEL_LINEARIMAGE_TEXTURE_HPP

#include "pastel/gfx/linearimage_texture.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Type, int N>
	Type sampleLinear(
		const Vector<real, N>& uv,
		const Array<Type, N>& image,
		const ArrayExtender<N, Type>& extender)
	{
		if (image.empty())
		{
			return Type();
		}

		// Gather all the neighboring 2^n samples.

		const integer n = uv.size();
		const integer samples = 1 << n;

		Vector<integer, N> x = floor(uv - 0.5);
		Vector<real, N> st = (uv - 0.5) - Vector<real, N>(x);
		Tuple<Type, ModifyN<N, 1 << N>::Result> valueSet(ofDimension(samples));
		Tuple<bool, N> s(ofDimension(samples), false);
		
		for (integer i = 0;i < samples;++i)
		{
			valueSet[i] = extender(image, x);

			integer axis = 0;
			while(axis < N)
			{
				if (s[axis])
				{
					s[axis] = false;
					--x[axis];
				}
				else
				{
					s[axis] = true;
					++x[axis];
					break;
				}
				++axis;
			}
		}

		return linear(st, 
			randomAccessRange(valueSet.begin(), valueSet.end()));
	}

}

#endif
