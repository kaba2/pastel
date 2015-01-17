#ifndef PASTELMATH_QR_DECOMPOSITION_HPP
#define PASTELMATH_QR_DECOMPOSITION_HPP

#include "pastel/math/qr_decomposition.h"
#include "pastel/math/householder_reflection.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	QrDecomposition<Real>::QrDecomposition(integer m, integer n)
		: q_(m, m)
		, r_(m, n)
	{
		ENSURE_OP(m, >=, 0);
		ENSURE_OP(n, >=, 0);
	}

	template <typename Real>
	QrDecomposition<Real>::QrDecomposition(
		Matrix<Real> that)
		: q_(that.m(), that.m())
		, r_(std::move(that))
	{
		decompose();
	}

	template <typename Real>
	QrDecomposition<Real>::QrDecomposition(
		const QrDecomposition& that)
		: q_(that.q_)
		, r_(that.r_)
	{
	}

	template <typename Real>
	QrDecomposition<Real>::QrDecomposition(
		QrDecomposition&& that)
		: q_(that.m(), that.m())
		, r_(that.m(), that.n())
	{
		swap(that);
	}

	template <typename Real>
	integer QrDecomposition<Real>::m() const
	{
		return q_.m();
	}

	template <typename Real>
	integer QrDecomposition<Real>::n() const
	{
		return r_.n();
	}

	template <typename Real>
	void QrDecomposition<Real>::swap(
		QrDecomposition& that)
	{
		q_.swap(that.q_);
		r_.swap(that.r_);
	}

	template <typename Real>
	QrDecomposition<Real>& QrDecomposition<Real>::operator=(
		QrDecomposition that)
	{
		swap(that);
		return *this;
	}

	template <typename Real>
	const Matrix<Real>& QrDecomposition<Real>::qTransposed() const
	{
		return q_;
	}

	template <typename Real>
	const Matrix<Real>& QrDecomposition<Real>::r() const
	{
		return r_;
	}

	template <typename Real>
	void QrDecomposition<Real>::decompose(Matrix<Real> that)
	{
		integer m = that.m();

		q_ = identityMatrix<Real>(m, m);
		r_ = std::move(that);

		decompose();
	}

	// Private

	template <typename Real>
	void QrDecomposition<Real>::decompose()
	{

		/*
		QR decomposition
		================
		
		Let
		alpha = -sign(x_1) |x|
		v = x - alpha e_1
		Q = I - 2vv^T.
		
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
		
		integer m = r_.m();
		integer n = r_.n();

		// We will reuse the memory space for v
		// to avoid reallocation.
		Vector<Real> v(ofDimension(m));

		for (integer j = 0;j < n - 1;++j)
		{
			// Consider the (m - j) x (n - j) minor at
			// the bottom right.

			// Note that for each j we only use
			// the components v[j], ..., v[m - 1] of v.
			for (integer i = j;i < m;++i)
			{
				v[i] = r_(i, j);
			}

			// Compute R := Q_{j + 1} ... Q_1 A.
			householder(r_, v, j, j);
			// Compute Q^T := Q_{j + 1} ... Q_1.
			householder(q_, v, j, 0);
		}
	}

}

#include "pastel/math/matrix_diagonal_product.h"
#include "pastel/math/solve_linear.h"

namespace Pastel
{

	template <typename Real>
	Real absDeterminant(
		const QrDecomposition<Real>& qr)
	{
		return mabs(diagonalProduct(qr.r()));
	}

	template <typename Real, int N, typename Expression>
	Vector<Real> solveLinear(
		const QrDecomposition<Real>& qr,
		const VectorExpression<Real, N, Expression>& b)
	{
		ENSURE_OP(qr.n(), ==, b.size());
		
		integer n = b.size();

		// QRx = b
		
		Vector<Real> x(ofDimension(n));

		// First solve Qy = b.

		x = qr.qTransposed() * b;
		
		// Then solve Rx = y.
		x = solveUpperTriangular(qr.r(), x);

		return x;
	}

}

#endif
