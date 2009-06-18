#ifndef PASTEL_QR_DECOMPOSITION_H
#define PASTEL_QR_DECOMPOSITION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <int N, typename Real>
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
			const MatrixExpression<N, N, Real, Expression>& that);

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
			const MatrixExpression<N, N, Real, Expression>& that);

		//! Returns the transpose of the orthogonal matrix.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<N, N, Real>& qTransposed() const;

		//! Returns the upper triangular matrix.
		/*!
		Exception safety:
		nothrow

		Time complexity:
		constant
		*/
		const Matrix<N, N, Real>& r() const;

	private:
		Matrix<N, N, Real> q_;
		Matrix<N, N, Real> r_;
	};

}

#include "pastel/math/qr_decomposition.hpp"

#endif
