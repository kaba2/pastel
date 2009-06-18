#ifndef PASTEL_MATRIX_TOOLS_MORE2_HPP
#define PASTEL_MATRIX_TOOLS_MORE2_HPP

#include "pastel/math/matrix_tools.h"
#include "pastel/math/matrix.h"
#include "pastel/math/orthonormal.h"
#include "pastel/math/qr_decomposition.h"

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

		matrix.set(0);

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

		matrix.set(0);

		for (integer i = 0;i < size;++i)
		{
			result(i, i) = values[i];
		}
	}

	template <int N, typename Real>
	void setRandomRotation(
		Matrix<N, N, Real>& result)
	{
		// See "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the ACM,
		// Volume 54, Number 5.

		ENSURE2(result.width() == result.height(),
			result.width(), result.height());

		const integer n = result.width();
		
		const Real scaling = 
			inverse(std::sqrt((Real)2));

		for (integer y = 0;y < n;++y)
		{
			for (integer x = 0;x < n;++x)
			{
				result(y, x) = randomGaussian<Real>() * scaling;
			}
		}

		const QrDecomposition<N, Real> qr(result);

		Vector<N, Real> rSign = diagonal(qr.r());
		for (integer i = 0;i < n;++i)
		{
			if (rSign[i] < 0)
			{
				rSign[i] = -1;
			}
			else
			{
				rSign[i] = 1;
			}
		}
		
		result = qr.qTransposed() * diagonal(rSign) * qr.qTransposed();
	}

	template <int Height, int Width, typename Real>
	void setRandomReducedRotation(
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
		determinant d.

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
		Now we just need to generate D such that det(D) = d. 
		Note that all symmetric positive
		definite matrices can be brought to the form
		of S by eigendecomposition. Let
		
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

		Clearly the diagonal elements of D
		are positive.
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

	template <int N, typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		const PASTEL_NO_DEDUCTION(Real)& condition,
		Matrix<N, N, Real>& result)
	{
		/*
		Problem:
		Generate a random symmetric positive 
		definite (n x n)-matrix	S with 
		determinant d and condition number c.

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
		Now we just need to generate D such that det(S) = d,
		and cond(S) = c. Note that all symmetric positive
		definite matrices can be brought to the form
		of S by eigendecomposition.

		Let D = diag(e^(-a), e^(-b), e^(-b), ..., e^(-b)).
		and assume a >= b.
		
		Then

		cond(S) 
		= cond(D) 
		= e^(-b) / e^(-a)
		= e^a / e^b
		= e^(a - b)

		det(S)
		= det(D)
		= e^(-a) e^(-b(n - 1))
		
		Thus we get the pair of equations:
		(1) cond(S) = e^(a - b) = c
		(2) det(S) = e^(-a) e^(-b(n - 1)) = d
		
		(1)
		<=>
		a - b = ln(c)
		<=>
		b = a - ln(c)

		(2)
		<=>
		-a - b(n - 1) = ln(d)
		<=>
		a + b(n - 1) = -ln(d)
		(3)
		
		(1) -> (3)
		<=>
		a + (a - ln(c))(n - 1) = -ln(d)
		<=>
		an - (n - 1) ln(c) = -ln(d)
		<=>
		a = [(n - 1) ln(c) - ln(d)] / n
		<=>
		(4)

		(4) -> (1)
		<=>
		b = [(n - 1) ln(c) - ln(d) - n ln(c)] / n
		= [-ln(c) - ln(d)] / n
		= -[ln(c) + ln(d)] / n
		
		Thus the solution is:
		a = [(n - 1) ln(c) - ln(d)] / n
		b = -[ln(c) + ln(d)] / n

		We assumed 
		a >= b
		<=>
		a - b >= 0
		<=>
		ln(c) >= 0
		<=>
		c >= 1

		This is the only restriction.
		*/

		ENSURE2(result.width() == result.height(),
			result.width(), result.height());
		ENSURE(condition >= 1);

		const integer n = result.width();

		const Real a = 
			((n - 1) * std::log(condition) - 
			std::log(determinant)) / n;
		
		const Real b =
			a - std::log(c);

		setRandomRotation(result);

		result[0] *= std::exp(-a / 2);
		for (integer i = 1;i < n;++i)
		{
			result[i] *= std::exp(-b / 2);
		}

		result *= transpose(result);
	}

}


#endif
