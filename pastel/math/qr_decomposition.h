// Description: QrDecomposition class
// Detail: Computes the QR-decomposition of a matrix

#ifndef PASTELMATH_QR_DECOMPOSITION_H
#define PASTELMATH_QR_DECOMPOSITION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class QrDecomposition
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! A qr decomposition of the identity.
		/*!
		Both q and r are set to identity matrices.

		Exception safety:
		basic

		Time complexity:
		O(n^2)
		*/
		QrDecomposition();
		
		//! Decomposes the given matrix expression.
		/*!
		Precondition:
		that.width() == that.height()

		Exception safety:
		basic

		Time complexity:
		O(n^3)
		*/
		template <typename Expression>
		explicit QrDecomposition(
			const MatrixExpression<Real, N, N, Expression>& that);

		//! Returns the width of the decomposed matrix.
		integer width() const;

		//! Returns the height of the decomposed matrix.
		integer height() const;

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

		//! Decomposes the given matrix expression.
		/*!
		Precondition:
		that.width() == that.height()

		Exception safety:
		basic

		Time complexity:
		O(n^3)
		*/
		template <typename Expression>
		void decompose(
			const MatrixExpression<Real, N, N, Expression>& that);

		//! Returns the transpose of the orthogonal matrix.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<Real, N, N>& qTransposed() const;

		//! Returns the upper triangular matrix.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<Real, N, N>& r() const;

	private:
		Matrix<Real, N, N> q_;
		Matrix<Real, N, N> r_;
	};

	typedef QrDecomposition<real, 1> QrDecomposition1;
	typedef QrDecomposition<real, 2> QrDecomposition2;
	typedef QrDecomposition<real, 3> QrDecomposition3;
	typedef QrDecomposition<real, 4> QrDecomposition4;
	typedef QrDecomposition<real, Dynamic> QrDecompositionD;

}

#include "pastel/math/qr_decomposition.hpp"

#endif
