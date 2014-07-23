#ifndef PASTELMATH_ORTHONORMAL_HPP
#define PASTELMATH_ORTHONORMAL_HPP

#include "pastel/math/orthonormal.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> orthonormalize(Matrix<Real> matrix)
	{
		if (matrix.size() == 0)
		{
			return matrix;
		}

		integer n = matrix.n();
		for (integer i = 0;i < n;++i)
		{
			// We will retain the loop-invariant that the
			// first i columns form an orthonormal set.

			for (integer j = 0;j < i;++j)
			{
				// From the i:th column, remove the contributions of the
				// vectors in matrix[0]..matrix[i - 1].

				// Note: While equivalent mathematically, 
				// it is important for numerical stability
				// that we use the modified vectors in the dot product
				// computation, rather than the original ones.
				matrix.column(i) -= 

					matrix.column(j) * dot(matrix.column(i), matrix.column(j));
			}

			Real vNorm = norm(matrix.column(i));

			// EPSILON
			if (vNorm == 0)
			{
				throw SingularMatrix_Exception();
			}

			// Normalize to unit length.
			matrix.column(i) /= vNorm;
		}

		return matrix;
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

		ENSURE_OP(orthonormalSet.front().n(), ==, dimension);
		
		integer vectors = orthonormalSet.size();

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
