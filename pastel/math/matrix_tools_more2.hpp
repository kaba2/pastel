#ifndef PASTEL_MATRIX_TOOLS_MORE2_HPP
#define PASTEL_MATRIX_TOOLS_MORE2_HPP

#include "pastel/math/matrix_tools.h"
#include "pastel/math/matrix.h"
#include "pastel/math/orthonormal.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/random_vector.h"
#include "pastel/sys/mytypes.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	template <int Height, int Width, typename Real>
	void setRandomMatrix(
		Matrix<Height, Width, Real>& matrix)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		for (integer i = 0;i < height;++i)
		{
			for (integer j = 0;j < width;++j)
			{
				matrix(i, j) = random<Real>() * 2 - 1;
			}
		}
	}

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		ENSURE2(width == height, width, height);

		for (integer i = 0;i < width;++i)
		{
			matrix(i, i) = value;
		}
	}

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& values)
	{
		const integer size = values.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		for (integer i = 0;i < size;++i)
		{
			result(i, i) = values[i];
		}
	}

	// Householder

	template <int N, typename Real>
	void setHouseHolder(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& v)
	{
		const integer size = v.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		for (integer i = 0;i < size;++i)
		{
			for (integer j = 0;j < size;++j)
			{
				matrix(i, j) = v[j] * -2 * v[i];
			}

			matrix(i, i) += 1;
		}
	}

	template <int Height, int Width, typename Real>
	void setRandomRotation(
		Matrix<Height, Width, Real>& result)
	{
		const integer height = result.height();
		const integer width = result.width();

		ENSURE2(height <= width, height, width);

		std::vector<Vector<Width, Real> > orthonormalSet;
		orthonormalSet.reserve(height);

		orthonormalSet.push_back(
			randomVectorSphere<Width, Real>(width));
		result[0] = orthonormalSet.back();

		for (integer i = 1;i < height;++i)
		{
			orthonormalSet.push_back(
				perpendicular(width, orthonormalSet));
			result[i] = orthonormalSet.back();
		}
	}

	template <int N, typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		Matrix<N, N, Real>& result)
	{
		/*
		Problem:
		Generate a random symmetric positive 
		definite (n x n)-matrix	S with 
		determinant d in ]0, 1].

		Solution:
		Let R be an orthogonal (n x n)-matrix.
		Let D be a positive (n x n)-diagonal matrix.
		Let S = RDR^T.

		Then 
		
		S is symmetric:

		S^T = (RDR^T)^T = R D^T R^T = RDR^T = S

		S is positive definite:

		(for all x in R^n:)
		x^T S x > 0
		<=>
		x^T R D R^T x > 0
		<=> (R is invertible)
		x^T D x > 0
		<=> (D is positive diagonal)
		true

		det(S) 
		= det(RDR^T) 
		= det(R) det(D) det(R^T)
		= det(D)

		The generation of a random rotation matrix R
		is described in the function 'setRandomRotation'.
		Now we just need to generate D such that det(D) = d,
		with d in ]0, 1]. To accomplish this, let
		
		a_1 = 0
		a_(n+1) = -ln(d)
		and let
		{a_2, ..., a_n}
		
		be a set of n uniform random numbers in [0, -ln(d)].
		Sort this list to ascending order with a 
		permutation p. Define

		b_i = a_p(i + 1) - a_p(i)
		
		Clearly 
		
		sum b_i = -ln(D).

		We claim D can be formed by setting

		D = diag(e^(-b_1), ..., e^(-b_n))

		To check the claim, compute:
		
		det(D) 
		= prod e^(-b_i)
		= e^(sum -b_i)
		= e^(-sum b_i)
		= e^(ln(d))
		= d

		Because of the exponentiation, the
		diagonal elements of D are all > 0.
		Because b_i are all non-negative, 
		the diagonal elements of D all
		lie in the interval ]0, 1].
		*/

		const integer n = result.width();

		ENSURE2(result.width() == result.height(),
			result.width(), result.height());

		// Generate a random partition of the
		// interval [0, -ln(d)] to n subintervals
		// represented by (n + 1) partition points.

		const Real xDelta = -std::log(determinant);

		std::vector<Real> partitionSet;
		partitionSet.reserve(n + 1);
		partitionSet.push_back(0);
		partitionSet.push_back(xDelta);
		
		for (integer i = 0;i < n - 1;++i)
		{
			partitionSet.push_back(random<Real>() * xDelta);
		}

		std::sort(partitionSet.begin(), partitionSet.end());

		// Generate a random rotation matrix.

		setRandomRotation(result);

		// Multiply the rows of the rotation matrix
		// with square root of the diagonal element of D.

		for (integer i = 0;i < n;++i)
		{
			const Real b = partitionSet[i + 1] - partitionSet[i];
			result[i] *= std::exp(-b / 2);
		}

		result *= transpose(result);
	}

}


#endif
