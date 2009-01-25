#ifndef PASTELGEOMETRY_FLATTOOLS_HPP
#define PASTELGEOMETRY_FLATTOOLS_HPP

#include "pastel/geometry/flat_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real, int M>
	bool orthonormalize(
		const Flat<N, Real, M>& input,
		Flat<N, Real, M>& output)
	{
		// Stabilized Gram-Schmidt orthonormalization

		const integer vectors = input.size();

		for (integer i = 0;i < vectors;++i)
		{
			Vector<N, Real> v(input[i]);

			for (integer j = 0;j < i;++j)
			{
				v -= output[j] * dot(v, output[j]);
			}

			const Real vNorm(norm(v));

			// EPSILON
			if (vNorm == 0)
			{
				return false;
			}

			output[i] = v / vNorm;
		}

		return true;
	}

	template <int N, typename Real, int M>
	bool orthonormalize(
	    Flat<N, Real, M>& flat)
	{
		return orthonormalize(flat, flat);
	}

	template <int N, typename Real, int M>
	Vector<N, Real> perpendicular(
		const Flat<N, Real, M>& flat)
	{
		// flat is assumed to be an orthonormal set of vectors

		if (M >= N)
		{
			return Vector<N, Real>(0);
		}

		// Find the positive axis-aligned vector e_i
		// that has the minimum maximum absolute dot product
		// with the set flat.

		integer minMaxDotIndex = 0;
		Real minMaxDot = infinity<Real>();

		for (integer i = 0;i < N;++i)
		{
			Real maxDot(0);
			for (integer j = 0;j < vectorCount;++j)
			{
				const Real absDot(std::abs(flat[j][i]));
				if (absDot > maxDot)
				{
					maxDot = absDot;
				}
			}

			if (maxDot < minMaxDot)
			{
				minMaxDot = maxDot;
				minMaxDotIndex = i;
			}
		}

		// Make e_i orthogonal to flat

		Vector<N, Real> result(unitAxis<N, Real>(minMaxDotIndex));

		for (integer i = 0;i < M;++i)
		{
			// Remove flat[i] directed contribution

			result -= flat[i] * dot(result, flat[i]);
		}

		// Normalize to unit length

		result /= norm(result);

		return result;
	}

}

#endif
