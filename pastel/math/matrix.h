/*!
\file
\brief A class for a real matrix.
*/

#ifndef PASTEL_MATRIX_H
#define PASTEL_MATRIX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/array.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include "pastel/math/matrixbase.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	//! A matrix.

	/*!
	You can create two kinds of matrices:
	statically and dynamically sized.
	Matrix<3, 4, real> is an example of
	a statically sized matrix, while
	Matrix<Dynamic, Dynamic, real> is
	a dynamically sized matrix.
	Matrices use the same algorithms independent
	of whether they are static or dynamic sized.
	*/

	template <int Height, int Width, typename Real>
	class Matrix
		: public Detail::MatrixBase<Height, Width, Real>
	{
	private:
		typedef Detail::MatrixBase<Height, Width, Real> Base;
	public:

		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: Base(that)
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return Base::operator=(that);
		}

		// Used for concept checking.
		~Matrix()
		{
			BOOST_STATIC_ASSERT(Height > 0 || Height == Dynamic);
			BOOST_STATIC_ASSERT(Width > 0 || Width == Dynamic);
			BOOST_STATIC_ASSERT((Width == Dynamic && Height == Dynamic) ||
				(Width != Dynamic && Height != Dynamic));
		}
	};

	template <typename Real>
	class Matrix<Dynamic, Dynamic, Real>
		: public MatrixExpression<Dynamic, Dynamic, Real, 
		Matrix<Dynamic, Dynamic, Real> >
	{
	public:
		typedef const Matrix& StorageType;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Matrix();

		template <int Height, int Width, typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that);

		Matrix(integer height, integer width);

		void swap(Matrix& that);
		void clear();

		void setSize(integer height, integer width);
		integer width() const;
		integer height() const;

		void set(const Real& that);

		bool involves(void* address) const;
		bool involvesNonTrivially(void* address) const;

		Real& operator()(integer y, integer x);
		const Real& operator()(integer y, integer x) const;

		TemporaryVector<Dynamic, Real> operator[](integer y);
		const TemporaryVector<Dynamic, Real> operator[](integer y) const;

		template <typename Expression>
		Matrix<Dynamic, Dynamic, Real>& operator=(
			const MatrixExpression<Dynamic, Dynamic, Real, Expression>& right);

		Matrix<Dynamic, Dynamic, Real>& operator*=(
			const Real& that);
		Matrix<Dynamic, Dynamic, Real>& operator/=(
			const Real& that);

		template <typename Expression>
		Matrix<Dynamic, Dynamic, Real>& operator*=(
			const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that);

		template <typename Expression>
		Matrix<Dynamic, Dynamic, Real>& operator+=(
			const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that);

		template <typename Expression>
		Matrix<Dynamic, Dynamic, Real>& operator-=(
			const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that);

	private:
		Array<2, Real> data_;
	};

	template <typename Real>
	class Matrix<1, 1, Real>
		: public Detail::MatrixBase<1, 1, Real>
	{
	private:
		enum
		{
			Height = 1,
			Width = 1
		};

		typedef Detail::MatrixBase<Height, Width, Real> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: Base(that)
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		//! Constructs with the given row vector.
		Matrix(
			const Vector<1, Real>& firstRow)
			: Base()
		{
			(*this)[0] = firstRow;
		}

		//! Constructs with the given element.
		explicit Matrix(
			const Real& m00)
			: Base()
		{
			Matrix& m = *this;
			m(0, 0) = m00;
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return Base::operator=(that);
		}

		//! Sets the element of the matrix.
		void set(
			const Real& m00)
		{
			Matrix& m = *this;
			m(0, 0) = m00;
		}
	};

	template <typename Real>
	class Matrix<2, 2, Real>
		: public Detail::MatrixBase<2, 2, Real>
	{
	private:
		enum
		{
			Height = 2,
			Width = 2
		};

		typedef Detail::MatrixBase<Height, Width, Real> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: Base(that)
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		//! Constructs with the given row vectors.
		Matrix(
			const Vector<2, Real>& firstRow,
			const Vector<2, Real>& secondRow)
			: Base()
		{
			(*this)[0] = firstRow;
			(*this)[1] = secondRow;
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01,
			const Real& m10, const Real& m11)
			: Base()
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(1, 0) = m10;
			m(1, 1) = m11;
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return Base::operator=(that);
		}

		//! Sets the elements of the matrix.
		void set(
			const Real& m00, const Real& m01,
			const Real& m10, const Real& m11)
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(1, 0) = m10;
			m(1, 1) = m11;
		}
	};

	template <typename Real>
	class Matrix<3, 3, Real>
		: public Detail::MatrixBase<3, 3, Real>
	{
	private:
		enum
		{
			Height = 3,
			Width = 3
		};

		typedef Detail::MatrixBase<Height, Width, Real> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: Base(that)
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		//! Constructs with the given row vectors.
		Matrix(
			const Vector<3, Real>& firstRow,
			const Vector<3, Real>& secondRow,
			const Vector<3, Real>& thirdRow)
			: Base()
		{
			(*this)[0] = firstRow;
			(*this)[1] = secondRow;
			(*this)[2] = thirdRow;
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01, const Real& m02,
			const Real& m10, const Real& m11, const Real& m12,
			const Real& m20, const Real& m21, const Real& m22)
			: Base()
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(0, 2) = m02;
			m(1, 0) = m10;
			m(1, 1) = m11;
			m(1, 2) = m12;
			m(2, 0) = m20;
			m(2, 1) = m21;
			m(2, 2) = m22;
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return Base::operator=(that);
		}

		//! Sets the elements of the matrix.
		void set(
			const Real& m00, const Real& m01, const Real& m02,
			const Real& m10, const Real& m11, const Real& m12,
			const Real& m20, const Real& m21, const Real& m22)
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(0, 2) = m02;
			m(1, 0) = m10;
			m(1, 1) = m11;
			m(1, 2) = m12;
			m(2, 0) = m20;
			m(2, 1) = m21;
			m(2, 2) = m22;
		}
	};

	template <typename Real>
	class Matrix<4, 4, Real>
		: public Detail::MatrixBase<4, 4, Real>
	{
	private:
		enum
		{
			Height = 4,
			Width = 4
		};

		typedef Detail::MatrixBase<Height, Width, Real> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: Base(that)
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		//! Constructs with the given row vectors.
		Matrix(
			const Vector<4, Real>& firstRow,
			const Vector<4, Real>& secondRow,
			const Vector<4, Real>& thirdRow,
			const Vector<4, Real>& fourthRow)
			: Base()
		{
			(*this)[0] = firstRow;
			(*this)[1] = secondRow;
			(*this)[2] = thirdRow;
			(*this)[3] = fourthRow;
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01,
			const Real& m02, const Real& m03,
			const Real& m10, const Real& m11,
			const Real& m12, const Real& m13,
			const Real& m20, const Real& m21,
			const Real& m22, const Real& m23,
			const Real& m30, const Real& m31,
			const Real& m32, const Real& m33)
			: Base()
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(0, 2) = m02;
			m(0, 3) = m03;
			m(1, 0) = m10;
			m(1, 1) = m11;
			m(1, 2) = m12;
			m(1, 3) = m13;
			m(2, 0) = m20;
			m(2, 1) = m21;
			m(2, 2) = m22;
			m(2, 3) = m23;
			m(3, 0) = m30;
			m(3, 1) = m31;
			m(3, 2) = m32;
			m(3, 3) = m33;
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return Base::operator=(that);
		}

		//! Sets the elements of the matrix.
		void set(
			const Real& m00, const Real& m01,
			const Real& m02, const Real& m03,
			const Real& m10, const Real& m11,
			const Real& m12, const Real& m13,
			const Real& m20, const Real& m21,
			const Real& m22, const Real& m23,
			const Real& m30, const Real& m31,
			const Real& m32, const Real& m33)
		{
			Matrix& m = *this;
			m(0, 0) = m00;
			m(0, 1) = m01;
			m(0, 2) = m02;
			m(0, 3) = m03;
			m(1, 0) = m10;
			m(1, 1) = m11;
			m(1, 2) = m12;
			m(1, 3) = m13;
			m(2, 0) = m20;
			m(2, 1) = m21;
			m(2, 2) = m22;
			m(2, 3) = m23;
			m(3, 0) = m30;
			m(3, 1) = m31;
			m(3, 2) = m32;
			m(3, 3) = m33;
		}
	};

	typedef Matrix<Dynamic, Dynamic, real> DynamicMatrix;
	typedef Matrix<1, 1, real> Matrix1;
	typedef Matrix<2, 2, real> Matrix2;
	typedef Matrix<3, 3, real> Matrix3;
	typedef Matrix<4, 4, real> Matrix4;

	// Vectors vs matrices

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Height, Real, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Height, Width, Real, LeftExpression>& left,
		const VectorExpression<Width, Real, RightExpression>& right);

	template <
		int N,
		typename Real,
		typename VectorExpression,
		typename MatrixExpression>
	class VectorMatrixMultiplication;

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Width, Real, LeftExpression, RightExpression> operator *(
		const VectorExpression<Height, Real, LeftExpression>& left,
		const MatrixExpression<Height, Width, Real, RightExpression>& right);

	// Points vs matrices

	template <int Height, int Width, typename Real>
	Point<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Width, Real>& right);

	template <int Height, int Width, typename Real>
	Point<Width, Real> operator *(
		const Point<Height, Real>& left,
		const Matrix<Height, Width, Real>& right);

	template <int Height, int Width, typename Real>
	void swap(
		Matrix<Height, Width, Real>& left,
		Matrix<Height, Width, Real>& right);

}

#include "pastel/math/matrix.hpp"

#endif
