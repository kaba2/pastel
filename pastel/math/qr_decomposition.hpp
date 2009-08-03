#ifndef PASTEL_QR_DECOMPOSITION_HPP
#define PASTEL_QR_DECOMPOSITION_HPP

#include "pastel/math/qr_decomposition.h"
#include "pastel/math/matrix_tools.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	QrDecomposition<Real, N>::QrDecomposition()
		: q_()
		, r_()
	{
	}

	template <typename Real, int N>
	template <typename Expression>
	QrDecomposition<Real, N>::QrDecomposition(
		const MatrixExpression<N, N, Real, Expression>& that)
		: q_()
		, r_()
	{
		decompose(that);
	}

	template <typename Real, int N>
	integer QrDecomposition<Real, N>::width() const
	{
		return q_.width();
	}

	template <typename Real, int N>
	integer QrDecomposition<Real, N>::height() const
	{
		return q_.height();
	}

	template <typename Real, int N>
	void QrDecomposition<Real, N>::swap(
		QrDecomposition& that)
	{
		q_.swap(that.q_);
		r_.swap(that.r_);
	}

	template <typename Real, int N>
	const Matrix<N, N, Real>& QrDecomposition<Real, N>::qTransposed() const
	{
		return q_;
	}

	template <typename Real, int N>
	const Matrix<N, N, Real>& QrDecomposition<Real, N>::r() const
	{
		return r_;
	}

	template <typename Real, int N>
	template <typename Expression>
	void QrDecomposition<Real, N>::decompose(
		const MatrixExpression<N, N, Real, Expression>& that)
	{
		ENSURE_OP(that.width(), ==, that.height());

		/*
		QR decomposition
		================

		A Householder reflection w.r.t. v is given by:
		
		Q = I - 2vv^T / (v^T v)

		It is a reflection of a vector w.r.t. the hyperplane
		whose normal is v:

		Qx = (I - 2vv^T / (v^T v))x
		= x - (2v v^T x / (v^T v))
		= x - 2 v' <v', x>

		where v' = v / |v|

		The reflection property is confirmed by:

		<Qx, v'> 
		= <x, v'> - 2 <v', v'> <v', x>
		= -<x, v'>

		Q is symmetric:

		Q^T 
		= (I - 2vv^T / (v^T v))^T
		= I - 2vv^T / (v^T v)
		= Q
		
		Q is orthogonal:

		Q^T Q 
		= Q^2
		= (I - 2vv^T / (v^T v))^2
		= I - 4vv^T / (v^T v) + (4vv^T vv^T) / (v^T v)^2
		= I - 4vv^T / (v^T v) + (4v(v^T v)v^T) / (v^T v)^2
		= I - 4vv^T / (v^T v) + 4vv^T / v^T v
		= I
		
		Let
		alpha = -sign(x_1) |x|
		v = x - alpha e_1
		
		Then
		Qx = alpha e_1
		
		Given a matrix A, let its first column be x.
		Then QA has as its first column alpha e_1.
		Recurse by consider the minor you get by
		removing the first column and row from QA.
		
		In the end you end up with:

		Q_n ... Q_2 Q_1 A = R

		such that Q_i are orthogonal matrices
		and R is an upper triangular matrix.
		Here the Q_i have been augmented from minor
		Householder reflection k x k matrices to full 
		n x n orthogonal matrices by adding a leading 
		identity matrix.
		
		Denote
		Q' = (Q_n ... Q_2 Q_1)^-1 = Q_1^T  ... Q_n^T

		Then
		A = Q' R

		The trick to efficient implementation is to
		take advantage of the special form of the
		Householder reflection as well as that
		we can consider the minors recursively.
		
		At each multiplication with a Householder
		matrix with x some column of the current
		minor:

		x' = (I - 2vv^T / (v^T v))x
		= x - 2v (v^T x) / (v^T v)
		= x - c v <v, x>

		where
		c = 2 / (v^T v)

		We can trace the Q' matrix at the same
		time, if we start from the identity matrix and
		apply the same transformations to that
		as well.
		*/
		
		const integer n = that.width();

		r_ = that;
		q_ = identityMatrix<N, N, Real>(n, n);

		// We will reuse the memory space for v
		// to avoid reallocation.
		Vector<Real, N> v(ofDimension(n));

		for (integer k = 0;k < n - 1;++k)
		{
			// Consider the (n - k) x (n - k) minor at
			// the bottom right.

			// Note that for each k we only use
			// the components v[k], ..., v[n - 1]
			// of v.
			
			// Compute v = x as well as dot(x, x).

			Real xDot = 0;
			for (integer i = k;i < n;++i)
			{
				const Real value = r_(i, k);
				v[i] = value;
				xDot += square(value);
			}

			const Real xNorm = std::sqrt(xDot);
			
			// Compute alpha = -sign(|x|) |x|

			const Real alpha =
				v[k] >= 0 ? -xNorm : xNorm;
			
			// Compute v = x - alpha e_1.

			v[k] -= alpha;

			// <v>
			// = <x - alpha e_1>
			// = <x> - 2 alpha <x, e_1> + alpha^2 <e_1>
			// = <x> - 2 alpha x_1 + alpha^2
			// = 2(<x> - alpha x_1)
			//
			// c 
			// = 2 / <v>
			// = 1 / (<x> - alpha x_1)

			const Real c = inverse(xDot - alpha * r_(k, k));

			// We already know the multiplication
			// result of the first column. It is
			// alpha e_1.

			r_(k, k) = alpha;
			for (integer i = k + 1;i < n;++i)
			{
				r_(i, k) = 0;
			}
		
			// The other columns need to multiplied
			// in normal manner.

			for (integer j = k + 1;j < n;++j)
			{
				// Compute <v, x>.
				
				Real vxDot = 0;
				for (integer i = k;i < n;++i)
				{
					vxDot += v[i] * r_(i, j);
				}
				vxDot *= c;

				// Compute x' = x - c v <v, x>.
				for (integer i = k;i < n;++i)
				{
					r_(i, j) -= v[i] * vxDot;
				}
			}

			// We also need to multiply the
			// orthogonal matrix in parallel.
			// Note that we actually end up
			// with the transpose of the q
			// we are seeking.
			
			for (integer j = 0;j < n;++j)
			{
				// Compute <v, x>.
				
				Real vxDot = 0;
				for (integer i = k;i < n;++i)
				{
					vxDot += v[i] * q_(i, j);
				}
				vxDot *= c;

				// Compute x' = x - c v <v, x>.
				for (integer i = k;i < n;++i)
				{
					q_(i, j) -= v[i] * vxDot;
				}
			}
		}
	}

}

#endif
