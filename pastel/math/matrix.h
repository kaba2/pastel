/*!
\file
\brief A class for a real matrix.
*/

#ifndef PASTELMATH_MATRIX_H
#define PASTELMATH_MATRIX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/array.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	//! A Matrix constructor tag for transposition.

	class MatrixTransposeTag {};

}

#include "pastel/math/matrixbase.h"

namespace Pastel
{

	//! A matrix.

	/*!
	You can create two kinds of matrices:
	statically and dynamically sized.
	Matrix<3, 4, real> is an example of
	a statically sized matrix, while
	Matrix<Unbounded, Unbounded, real> is
	a dynamically sized matrix.
	Matrices use the same algorithms independent
	of whether they are static or dynamic sized.
	*/

	template <int Height, int Width, typename Real>
	class Matrix
		: public Detail::MatrixBase<Height, Width, Real, Matrix>
	{
	private:
		typedef Detail::MatrixBase<Height, Width, Real,
			Pastel::Matrix> Base;
	public:

		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE2(width == Width, width, Width);
			ENSURE2(height == Height, height, Height);
		}

		//! Constructs with the transpose of the given matrix.
		Matrix(const Matrix<Width, Height, Real>& that,
			MatrixTransposeTag tag)
			: Base(that, tag)
		{
		}

		// Used for concept checking.
		~Matrix()
		{
			BOOST_STATIC_ASSERT(Height > 0 || Height == Unbounded);
			BOOST_STATIC_ASSERT(Width > 0 || Width == Unbounded);
			BOOST_STATIC_ASSERT((Width == Unbounded && Height == Unbounded) ||
				(Width != Unbounded && Height != Unbounded));
		}
	};

	template <typename Real>
	class Matrix<Unbounded, Unbounded, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Matrix();
		Matrix(const Matrix& that, MatrixTransposeTag);

		void swap(Matrix& that);
		void clear();

		void setSize(integer height, integer width);
		integer width() const;
		integer height() const;

		Real& operator()(integer y, integer x);
		const Real& operator()(integer y, integer x) const;

		Matrix<Unbounded, Unbounded, Real>& operator*=(
			const PASTEL_NO_DEDUCTION(Real)& that);
		Matrix<Unbounded, Unbounded, Real>& operator/=(
			const PASTEL_NO_DEDUCTION(Real)& that);

		Matrix<Unbounded, Unbounded, Real>& operator*=(const Matrix& that);
		Matrix<Unbounded, Unbounded, Real>& operator+=(const Matrix& that);
		Matrix<Unbounded, Unbounded, Real>& operator-=(const Matrix& that);

	private:
		Array<2, Real> data_;
	};

	template <typename Real>
	class Matrix<1, 1, Real>
		: public Detail::MatrixBase<1, 1, Real, Matrix>
	{
	private:
		enum
		{
			Height = 1,
			Width = 1
		};

		typedef Detail::MatrixBase<Height, Width, Real,
			Pastel::Matrix> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE1(width == Width, width);
			ENSURE1(height == Height, height);
		}

		//! Constructs with the transpose of the given matrix.
		Matrix(const Matrix<Height, Width, Real>& that,
			MatrixTransposeTag tag)
			: Base(that, tag)
		{
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
		: public Detail::MatrixBase<2, 2, Real, Matrix>
	{
	private:
		enum
		{
			Height = 2,
			Width = 2
		};

		typedef Detail::MatrixBase<Height, Width, Real,
			Pastel::Matrix> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE1(width == Width, width);
			ENSURE1(height == Height, height);
		}

		//! Constructs with the transpose of the given matrix.
		Matrix(const Matrix<2, 2, Real>& that,
			MatrixTransposeTag tag)
			: Base(that, tag)
		{
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
		: public Detail::MatrixBase<3, 3, Real, Matrix>
	{
	private:
		enum
		{
			Height = 3,
			Width = 3
		};

		typedef Detail::MatrixBase<Height, Width, Real,
			Pastel::Matrix> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE1(width == Width, width);
			ENSURE1(height == Height, height);
		}

		//! Constructs with the transpose of the given matrix.
		Matrix(const Matrix<3, 3, Real>& that,
			MatrixTransposeTag tag)
			: Base(that, tag)
		{
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
		: public Detail::MatrixBase<4, 4, Real, Matrix>
	{
	private:
		enum
		{
			Height = 4,
			Width = 4
		};

		typedef Detail::MatrixBase<Height, Width, Real,
			Pastel::Matrix> Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		Matrix(integer height, integer width)
			: Base()
		{
			ENSURE1(width == Width, width);
			ENSURE1(height == Height, height);
		}

		//! Constructs with the transpose of the given matrix.
		Matrix(const Matrix<4, 4, Real>& that,
			MatrixTransposeTag tag)
			: Base(that, tag)
		{
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

	typedef Matrix<Unbounded, Unbounded, real> UnboundedMatrix;
	typedef Matrix<1, 1, real> Matrix1;
	typedef Matrix<2, 2, real> Matrix2;
	typedef Matrix<3, 3, real> Matrix3;
	typedef Matrix<4, 4, real> Matrix4;

	// Matrices vs matrices

	template <int LeftHeight, int LeftWidth,
		int RightWidth, typename Real>
		Matrix<LeftHeight, RightWidth, Real> operator*(
		const Matrix<LeftHeight, LeftWidth, Real>& left,
		const Matrix<LeftWidth, RightWidth, Real>& right);

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator+(
		const Matrix<Height, Width, Real>& left,
		const Matrix<Height, Width, Real>& right);

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator-(
		const Matrix<Height, Width, Real>& left,
		const Matrix<Height, Width, Real>& right);

	// Vectors vs matrices

	template <int Height, int Width, typename Real>
	Vector<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Vector<Width, Real>& right);

	template <int Height, int Width, typename Real>
	Vector<Width, Real> operator *(
		const Vector<Height, Real>& left,
		const Matrix<Height, Width, Real>& right);

	// Points vs matrices

	template <int Height, int Width, typename Real>
	Point<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Width, Real>& right);

	template <int Height, int Width, typename Real>
	Point<Width, Real> operator *(
		const Point<Height, Real>& left,
		const Matrix<Height, Width, Real>& right);

	// Matrices and scalars

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator/(
		const Matrix<Height, Width, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right);

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator*(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Matrix<Height, Width, Real>& right);

	template <int Height, int Width, typename Real>
	void swap(
		Matrix<Height, Width, Real>& left,
		Matrix<Height, Width, Real>& right);

}

#include "pastel/math/matrix.hpp"

#endif
