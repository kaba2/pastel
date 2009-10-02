#ifndef PASTEL_SIMPLEX_NOISE_HPP
#define PASTEL_SIMPLEX_NOISE_HPP

#include "pastel/gfx/simplex_noise.h"
#include "pastel/gfx/gradientfield.h"

#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	template <typename Real>
	Real simplexNoise(const PASTEL_NO_DEDUCTION(Real)& position)
	{
		return 0;
	}

	template <typename Real, int N>
	Real simplexNoise(const Vector<Real, N>& x)
	{
		const integer n = x.size();
		const Real c = std::sqrt((Real)(n + 1));
		const Real d = std::sqrt((Real)n / (n + 1));
		const Real s = (c - 1) / n;
		const Real q = s / c;
		const Real dInv = inverse(d);

		// Transform the point to the integer
		// lattice simplex space.

		Vector<Real, N> u = d * (x + s * sum(x));

		// Find out in which cube we are.

		Vector<integer, N> p = floor(u);
		Vector<Real, N> f = x - Vector<Real, N>(p);

		// In that cube, find out in which
		// simplex we are in.

		std::vector<KeyValue<Real, integer> > orderSet;
		orderSet.reserve(n);
		for (integer i = 0;i < n;++i)
		{
			orderSet.push_back(keyValue(f[i], i));
		}
		std::sort(orderSet.begin(), orderSet.end(), std::greater<KeyValue<Real, integer> >());

		// Map the min vertex back to the regular simplex space.

		Vector<Real, N> simplexMin = (Vector<Real, N>(p) - q * sum(p)) * dInv;
		f = x - simplexMin;

		// Sum the contributions from the surrounding
		// simplex vertices weighted with a symmetric
		// attenuation function.

		const Real r2Inv = std::pow((Real)4 / 3, (Real)(n - 1));

		Real value = 0;
	
		for (integer i = 0;i < n;++i)
		{
			const Real attenuation = 1 - dot(f) * r2Inv;

			if (attenuation > 0)
			{
				const Real vertexValue = gradientField<Real, N>()(p, f);
				value += attenuation * vertexValue;
			}

			const integer axis = orderSet[i].value();

			f += q * dInv;
			f[axis] -= dInv;
			++p[axis];
		}
		{
			const Real attenuation = 1 - dot(f) * r2Inv;

			if (attenuation > 0)
			{
				const Real vertexValue = gradientField<Real, N>()(p, f);
				value += attenuation * vertexValue;
			}
		}

		return (value + 1) / 2;
	}

}

#endif
