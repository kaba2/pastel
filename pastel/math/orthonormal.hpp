#ifndef PASTEL_ORTHONORMAL_HPP
#define PASTEL_ORTHONORMAL_HPP

#include "pastel/math/orthonormal.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool orthonormalize(
		const std::vector<Vector<N, Real> >& input,
		std::vector<Vector<N, Real> >& result)
	{
		// Stabilized Gram-Schmidt orthonormalization

		std::vector<Vector<N, Real> > output;

		if (input.empty())
		{
			output.swap(result);
			return true;
		}

		const integer vectors = input.size();

		for (integer i = 0;i < vectors;++i)
		{
			Vector<N, Real> v(input[i]);

			for (integer j = 0;j < i;++j)
			{
				v -= output[j] * dot(v, output[j]);
			}

			const Real vNorm = norm(v);

			// EPSILON
			if (vNorm == 0)
			{
				return false;
			}

			output.push_back(v / vNorm);
		}

		output.swap(result);

		return true;
	}

	template <int N, typename Real>
	bool orthonormalize(
		std::vector<Vector<N, Real> >& vectorSet)
	{
		return orthonormalize(vectorSet, vectorSet);
	}

	template <int N, typename Real>
	Vector<N, Real> perpendicular(
		const std::vector<Vector<N, Real> >& orthonormalSet)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		
		return Pastel::perpendicular(N, orthonormalSet);
	}

	template <int N, typename Real>
	Vector<N, Real> perpendicular(
		integer dimension,
		const std::vector<Vector<N, Real> >& orthonormalSet)
	{
		ENSURE1(dimension > 0, dimension);

		// 'orthonormalSet' is assumed to be an orthonormal set of vectors

		if (orthonormalSet.empty())
		{
			return unitAxis<N, Real>(dimension, 0);
		}

		ENSURE2(orthonormalSet.front().dimension() == dimension,
			orthonormalSet.front().dimension(), dimension);
		
		const integer vectors = orthonormalSet.size();

		if (vectors >= dimension)
		{
			return Vector<N, Real>(ofDimension(dimension), 0);
		}

		// Find the positive axis-aligned vector e_i
		// that has the minimum maximum absolute dot product
		// with the set flat.

		integer minMaxDotIndex = 0;
		Real minMaxDot = infinity<Real>();

		for (integer i = 0;i < dimension;++i)
		{
			Real maxDot = 0;
			for (integer j = 0;j < vectors;++j)
			{
				const Real absDot = mabs(orthonormalSet[j][i]);
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

		Vector<N, Real> result = unitAxis<N, Real>(dimension, minMaxDotIndex);

		for (integer i = 0;i < vectors;++i)
		{
			// Remove flat[i] directed contribution

			result -= orthonormalSet[i] * dot(result, orthonormalSet[i]);
		}

		// Normalize to unit length

		result /= norm(result);

		return result;
	}

}

#endif
