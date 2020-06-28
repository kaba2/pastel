#ifndef PASTELMATH_ORTHONORMAL_HPP
#define PASTELMATH_ORTHONORMAL_HPP

#include "pastel/math/matrix/orthonormal.h"

#include "pastel/sys/vector.h"
#include "pastel/math/matrix/matrix.h"
#include "pastel/sys/vector/vector_tools.h"

#include <vector>

namespace Pastel
{

	//! Orthonormalizes the columns of a matrix.
	/*!
	This is stabilized Gram-Schmidt orthonormalization.
	It is numerically ok; if you need more stability, 
	use the QR-decomposition instead. 

	matrix:
	An mxn-matrix Q.

	returns:
	A matrix Q such that Q^T Q = I.

	throws:
	SingularMatrix_Exception if the matrix has 
	effectively linearly dependent columns.
	*/
	template <typename Real, int M, int N>
	Matrix<Real, M, N> orthonormalize(Matrix<Real, M, N> matrix)
	{
		if (matrix.size() == 0)
		{
			return matrix;
		}

		integer n = matrix.cols();
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
				matrix.col(i) -= 
					matrix.col(j) * matrix.col(i).dot(matrix.col(j));
			}

			Real vNorm = norm(asVector(matrix.col(i)));

			// EPSILON
			if (vNorm == 0)
			{
				throw SingularMatrix_Exception();
			}

			// Normalize to unit length.
			matrix.col(i) /= vNorm;
		}

		return matrix;
	}

	//! Calculates a perpendicular to a set of vectors
	/*!
	Preconditions:
	Vectors in 'orthonormalSet' are orthonormal.

	If the set is bigger than the dimensionality, 
	a zero vector is returned. 
	This is because every set bigger than dimensionality 
	is linearly dependent. If the set is empty, a unit vector 
	of the form (1, 0, 0, ..., 0) is returned.

	See also:
	orthonormalize()
	*/
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
		Real minMaxDot = Infinity();

		for (integer i = 0;i < dimension;++i)
		{
			Real maxDot = 0;
			for (integer j = 0;j < vectors;++j)
			{
				Real absDot = abs(orthonormalSet[j][i]);
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
