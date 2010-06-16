#ifndef PASTEL_ORTHONORMAL_HPP
#define PASTEL_ORTHONORMAL_HPP

#include "pastel/math/orthonormal.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int Height, int Width>
	bool orthonormalize(
		Matrix<Real, Height, Width>& vectorSet)
	{
		// Stabilized Gram-Schmidt orthonormalization
		// This is numerically ok, however, a more stable
		// way to do this is via QR decomposition using
		// Householder transformations.

		if (vectorSet.size() == 0)
		{
			return true;
		}

		const integer height = vectorSet.height();
		for (integer i = 0;i < height;++i)
		{
			// From i:th vector, remove the contributions of the
			// orthonormal set of vectors in vectorSet[0]..vectorSet[i - 1].
			for (integer j = 0;j < i;++j)
			{
				// Note: While equivalent mathematically, 
				// it is important for numerical stability
				// that we use the modified vectors in the dot product
				// computation, rather than the original ones.
				vectorSet[i] -= 
					vectorSet[j] * dot(vectorSet[i], vectorSet[j]);
			}

			const Real vNorm = norm(vectorSet[i]);

			// EPSILON
			if (vNorm == 0)
			{
				return false;
			}

			// Normalize to unit length.
			vectorSet[i] /= vNorm;
		}

		return true;
	}


	template <typename Real, int N>
	bool orthonormalize(
		const std::vector<Vector<Real, N> >& input,
		std::vector<Vector<Real, N> >& result)
	{
		// Stabilized Gram-Schmidt orthonormalization

		std::vector<Vector<Real, N> > output;

		if (input.empty())
		{
			output.swap(result);
			return true;
		}

		const integer vectors = input.size();

		for (integer i = 0;i < vectors;++i)
		{
			Vector<Real, N> v(input[i]);

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

	template <typename Real, int N>
	bool orthonormalize(
		std::vector<Vector<Real, N> >& vectorSet)
	{
		return orthonormalize(vectorSet, vectorSet);
	}

	template <typename Real, int N>
	Vector<Real, N> perpendicular(
		const std::vector<Vector<Real, N> >& orthonormalSet)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		
		return Pastel::perpendicular(N, orthonormalSet);
	}

	template <typename Real, int N>
	Vector<Real, N> perpendicular(
		integer dimension,
		const std::vector<Vector<Real, N> >& orthonormalSet)
	{
		ENSURE_OP(dimension, >, 0);

		// 'orthonormalSet' is assumed to be an orthonormal set of vectors

		if (orthonormalSet.empty())
		{
			return unitAxis<Real, N>(dimension, 0);
		}

		ENSURE_OP(orthonormalSet.front().dimension(), ==, dimension);
		
		const integer vectors = orthonormalSet.size();

		if (vectors >= dimension)
		{
			return Vector<Real, N>(ofDimension(dimension), 0);
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

		Vector<Real, N> result = unitAxis<Real, N>(dimension, minMaxDotIndex);

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
