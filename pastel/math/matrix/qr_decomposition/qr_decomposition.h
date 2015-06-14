// Description: QR decomposition

#ifndef PASTELMATH_QR_DECOMPOSITION_H
#define PASTELMATH_QR_DECOMPOSITION_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	template <typename Real>
	class QrDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! A qr decomposition of the identity.
		/*!
		Preconditions:
		m >= 0
		n >= 0

		Both q and r are set to identity matrices.

		Exception safety:
		basic

		Time complexity:
		O(n^2)
		*/
		explicit QrDecomposition(integer m, integer n);
		
		//! Decomposes the given matrix.
		/*!
		Exception safety:
		basic

		Time complexity:
		O(n^3)
		*/
		QrDecomposition(Matrix<Real> that);

		//! Copy-constructs from another decomposition.
		QrDecomposition(const QrDecomposition& that);

		//! Move-constructs from another decomposition.
		QrDecomposition(QrDecomposition&& that);

		//! Returns the width of the decomposed matrix.
		integer n() const;

		//! Returns the height of the decomposed matrix.
		integer m() const;

		//! Swaps two decompositions.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant (dynamic matrices)
		n^2 (static matrices)		
		*/
		void swap(
			QrDecomposition& that);

		//! Assigns from another decomposition.
		QrDecomposition& operator=(QrDecomposition that);

		//! Decomposes the given matrix.
		/*!
		Exception safety:
		basic

		Time complexity:
		O(n^3)
		*/
		void decompose(Matrix<Real> that);

		//! Returns the orthogonal matrix Q^T.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<Real>& qTransposed() const;

		//! Returns the upper triangular matrix R.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<Real>& r() const;

	private:
		QrDecomposition() = delete;
		void decompose();

		Matrix<Real> q_;
		Matrix<Real> r_;
	};

}

namespace Pastel
{

	//! Computes the absolute determinant from a qr decomposition.
	/*!
	The determinant of the orthogonal matrix is either -1 or 1.
	It would be costly to find out which. However, the
	absolute value of the determinant can be found quickly
	from the upper triangular matrix.
	*/
	template <typename Real>
	Real absDeterminant(
		const QrDecomposition<Real>& qr);

	//! Solves the linear system QRx = b.
	template <typename Real, integer N, typename Expression>
	Vector<Real> solveLinear(
		const QrDecomposition<Real>& qr,
		const VectorExpression<Real, N, Expression>& b);

}

#include "pastel/math/matrix/qr_decomposition/qr_decomposition.hpp"

#endif
