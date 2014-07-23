#ifndef PASTELGFX_SIMPLEX_NOISE_HPP
#define PASTELGFX_SIMPLEX_NOISE_HPP

#include "pastel/gfx/simplex_noise.h"
#include "pastel/gfx/gradientfield.h"
#include "pastel/gfx/perlin_noise.h"

namespace Pastel
{

	template <typename Real>
	Real simplexNoise(const PASTEL_NO_DEDUCTION(Real)& position)
	{
		return perlinNoise<Real>(position);
	}

	template <typename Real, int N>
	Real simplexNoise(const Vector<Real, N>& x)
	{
		integer n = x.size();
		Real c = std::sqrt((Real)(n + 1));
		const Real d = std::sqrt((Real)n / (n + 1));
		const Real s = (c - 1) / n;
		const Real q = s / c;
		const Real dInv = inverse(d);

		// Transform the point to the integer
		// cube simplicial partitioning.


		const Vector<Real, N> u = d * (x + s * sum(x));

		// Find out in which integer cube we are.

		Vector<integer, N> p = floor(u);
		Vector<Real, N> f = u - Vector<Real, N>(p);

		// In that cube, find out in which
		// simplex we are in.

		using Pair = std::pair<Real, integer>;

		std::vector<Pair> orderSet;
		orderSet.reserve(n);
		for (integer i = 0;i < n;++i)
		{
			orderSet.emplace_back(f[i], i);
		}

		auto greater = [&](const Pair& left, const Pair& right)
		{
			if (left.first != right.first)
			{
				return left.first > right.first;
			}
			return left.second > right.second;
		};

		std::sort(orderSet.begin(), orderSet.end(), greater);

		// Map the min vertex back to the transformed simplex space
		// (out of the scalings along [1, ..., 1], this one maximizes
		// the regularity of the resulting simplices).

		Vector<Real, N> simplexMin = 

			(Vector<Real, N>(p) - q * sum(p)) * dInv;
		f = x - simplexMin;

		// Sum the contributions from the surrounding
		// simplex vertices weighted with a symmetric
		// attenuation function.

		Real r2Inv = std::pow((Real)4 / 3, (Real)(n - 1));

		Real value = 0;
		for (integer i = 0;i < n;++i)
		{

			const Real attenuation = 1 - dot(f) * r2Inv;

			if (attenuation > 0)
			{
				Real vertexValue = gradientField<Real, N>()(p, f);

				value += square(square(attenuation)) * vertexValue;
			}

			integer axis = orderSet[i].second;


			f += q * dInv;
			f[axis] -= dInv;
			++p[axis];
		}
		{
			const Real attenuation = 1 - dot(f) * r2Inv;

			if (attenuation > 0)
			{
				Real vertexValue = gradientField<Real, N>()(p, f);

				value += square(square(attenuation)) * vertexValue;
			}
		}

		return (value * 2 + 1) / 2;
	}

}

#endif
