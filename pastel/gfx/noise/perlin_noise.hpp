#ifndef PASTELGFX_PERLIN_NOISE_HPP
#define PASTELGFX_PERLIN_NOISE_HPP

#include "pastel/gfx/noise/perlin_noise.h"
#include "pastel/gfx/noise/gradientfield.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

#include "pastel/math/interpolation/smoothstep.h"

#include <array>

namespace Pastel
{

	template <typename Real>
	Real perlinNoise(const NoDeduction<Real>& position)
	{
		return Pastel::perlinNoise<Real, 1>(position);
	}

	template <typename Real, int N>
	Real perlinNoise(const Vector<Real, N>& position)
	{
		integer n = position.size();

		Vector<integer, N> floorPosition = floor(position);
		Vector<Real, N> minDelta = position - Vector<Real, N>(floorPosition);
		
		// Find out the contribution of each vertex of the
		// containing cube.
		std::array<Real, (1 << N)> vertexSet;
		
		integer j = 0;
		Vector<integer, N> p = floorPosition;
		Vector<Real, N> f = minDelta;
		uint32 state = 0;
		while(true)
		{
			vertexSet[j] = gradientField<Real, N>()(p, f);
			++j;

			integer axis = 0;
			uint32 mask = 1;

			while((state & mask) && axis < n)
			{
				state -= mask;
				--p[axis];
				f[axis] = minDelta[axis];
				
				++axis;
				mask <<= 1;
			}
			if (axis == n)
			{
				break;
			}

			state += mask;
			++p[axis];
			--f[axis];
		}

		// The interpolation coefficients are modified
		// by a quintic smoothstep function.
		Vector<Real, N> t(ofDimension(n));
		for (integer i = 0;i < n;++i)
		{
			t[i] = quinticSmoothStep(minDelta[i]);
		}

		// Linearly interpolate between the contributions
		// of the cube vertices.
		Real value = linear(t, range(
			vertexSet.begin(), vertexSet.end()));

		// Map the noise to the [0, 1] range.
		return (value + 1) / 2;
	}

}

#endif
