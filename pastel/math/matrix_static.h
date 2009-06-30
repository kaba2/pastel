#ifndef PASTEL_MATRIX_STATIC_H
#define PASTEL_MATRIX_STATIC_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	namespace Detail
	{

		template <int Height, int Width, typename Real, typename Derived>
		class MatrixStaticBase
			: public Detail::MatrixBase<Height, Width, Real, Derived>
		{
		private:
			typedef Detail::MatrixBase<Height, Width, Real, Derived> Base;

		public:
			MatrixStaticBase()
				: Base()
			{
				// Initialize to an identity matrix.
				set(0);
				
				const integer minExtent = std::min(width(), height());
				for (integer i = 0;i < minExtent;++i)
				{
					(*this)(i, i) = 1;
				}
			}

			template <typename Expression>
			MatrixStaticBase(const MatrixExpression<Height, Width, Real, Expression>& that)
				: Base()
			{
				*this = that;
			}

			template <typename Expression>
			MatrixStaticBase& operator=(
				const MatrixExpression<Height, Width, Real, Expression>& that)
			{
				return Base::operator=(that);
			}

			// Used for concept checking.
			~MatrixStaticBase()
			{
				BOOST_STATIC_ASSERT(Height > 0 && Width > 0);
			}

			MatrixStaticBase& operator=(
				const MatrixStaticBase& that)
			{
				ENSURE2(width() == that.width(),
					width(), that.width());
				ENSURE2(height() == that.height(),
					height(), that.height());

				std::copy(that.data(), that.data() + that.size(),
					data());

				return *this;
			}

			void swap(MatrixStaticBase& that)
			{
				const integer n = size();
				for (integer i = 0;i < n;++i)
				{
					using std::swap;
					swap(data_[i], that.data_[i]);
				}
			}

			integer width() const
			{
				return Width;
			}

			integer height() const
			{
				return Height;
			}

			integer size() const
			{
				return Width * Height;
			}

			Real* data()
			{
				return data_;
			}

			const Real* data() const
			{
				return data_;
			}

			// The parameter is passed by value
			// because it could refer to this matrix.
			void set(const Real that)
			{
				std::fill(
					data_, data_ + size(), that);
			}

		private:
			Real data_[Height * Width > 0 ? Height * Width : 1];
		};

	}

	template <int Height, int Width, typename Real>
	class Matrix
		: public Detail::MatrixStaticBase<Height, Width, Real,
		Matrix<Height, Width, Real> >
	{
	private:
		typedef Detail::MatrixStaticBase<Height, Width, Real,
			Matrix<Height, Width, Real> > Base;

	public:
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

		Matrix& operator=(
			const Matrix& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		template <typename Expression>
		Matrix& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix<Height, Width, Real>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<1, 1, Real>
		: public Detail::MatrixStaticBase<1, 1, Real,
		Matrix<1, 1, Real> >
	{
	private:
		enum
		{
			Height = 1,
			Width = 1
		};

		typedef Detail::MatrixStaticBase<Height, Width, Real,
			Matrix<1, 1, Real> > Base;

	public:
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
		template <typename FirstExpression>
		Matrix(
			const VectorExpression<1, Real, FirstExpression>& firstRow)
			: Base()
		{
			(*this)[0] = firstRow[0];
		}

		//! Constructs with the given element.
		explicit Matrix(
			const Real& m00)
			: Base()
		{
			Matrix& m = *this;
			m(0, 0) = m00;
		}

		Matrix& operator=(
			const Matrix& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix<Height, Width, Real>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<2, 2, Real>
		: public Detail::MatrixStaticBase<2, 2, Real,
		Matrix<2, 2, Real> >
	{
	private:
		enum
		{
			Height = 2,
			Width = 2
		};

		typedef Detail::MatrixStaticBase<Height, Width, Real,
		Matrix<2, 2, Real> > Base;

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
		template <
			typename FirstExpression, 
			typename SecondExpression>
		Matrix(
			const VectorExpression<2, Real, FirstExpression>& firstRow,
			const VectorExpression<2, Real, SecondExpression>& secondRow)
			: Base()
		{
			set(firstRow, secondRow);
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01,
			const Real& m10, const Real& m11)
			: Base()
		{
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m10;
			Base::data()[3] = m11;
		}

		Matrix& operator=(
			const Matrix& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix<Height, Width, Real>&)Base::operator=(that);
		}

		//! Sets the elements of the matrix.
		void set(
			const Real& m00, const Real& m01,
			const Real& m10, const Real& m11)
		{
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m10;
			Base::data()[3] = m11;
		}

		template <
			typename FirstExpression, 
			typename SecondExpression>
		void set(
			const VectorExpression<2, Real, FirstExpression>& firstRow,
			const VectorExpression<2, Real, SecondExpression>& secondRow)
		{
			Base::data()[0] = firstRow[0];
			Base::data()[1] = firstRow[1];
			Base::data()[2] = secondRow[0];
			Base::data()[3] = secondRow[1];
		}
	};

	template <typename Real>
	class Matrix<3, 3, Real>
		: public Detail::MatrixStaticBase<3, 3, Real,
		Matrix<3, 3, Real> >
	{
	private:
		enum
		{
			Height = 3,
			Width = 3
		};

		typedef Detail::MatrixStaticBase<Height, Width, Real,
		Matrix<3, 3, Real> > Base;

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
		template <
			typename FirstExpression, 
			typename SecondExpression,
			typename ThirdExpression>
		Matrix(
			const VectorExpression<3, Real, FirstExpression>& firstRow,
			const VectorExpression<3, Real, SecondExpression>& secondRow,
			const VectorExpression<3, Real, ThirdExpression>& thirdRow)
			: Base()
		{
			set(firstRow, secondRow, thirdRow);
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01, const Real& m02,
			const Real& m10, const Real& m11, const Real& m12,
			const Real& m20, const Real& m21, const Real& m22)
			: Base()
		{
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m02;
			
			Base::data()[3] = m10;
			Base::data()[4] = m11;
			Base::data()[5] = m12;
			
			Base::data()[6] = m20;
			Base::data()[7] = m21;
			Base::data()[8] = m22;
		}

		Matrix& operator=(
			const Matrix& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix<Height, Width, Real>&)Base::operator=(that);
		}

		//! Sets the elements of the matrix.
		void set(
			const Real& m00, const Real& m01, const Real& m02,
			const Real& m10, const Real& m11, const Real& m12,
			const Real& m20, const Real& m21, const Real& m22)
		{
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m02;
			
			Base::data()[3] = m10;
			Base::data()[4] = m11;
			Base::data()[5] = m12;
			
			Base::data()[6] = m20;
			Base::data()[7] = m21;
			Base::data()[8] = m22;
		}

		template <
			typename FirstExpression, 
			typename SecondExpression,
			typename ThirdExpression>
		void set(
			const VectorExpression<3, Real, FirstExpression>& firstRow,
			const VectorExpression<3, Real, SecondExpression>& secondRow,
			const VectorExpression<3, Real, ThirdExpression>& thirdRow)
		{
			Base::data()[0] = firstRow[0];
			Base::data()[1] = firstRow[1];
			Base::data()[2] = firstRow[2];
			
			Base::data()[3] = secondRow[0];
			Base::data()[4] = secondRow[1];
			Base::data()[5] = secondRow[2];
			
			Base::data()[6] = thirdRow[0];
			Base::data()[7] = thirdRow[1];
			Base::data()[8] = thirdRow[2];
		}
	};

	template <typename Real>
	class Matrix<4, 4, Real>
		: public Detail::MatrixStaticBase<4, 4, Real,
		Matrix<4, 4, Real> >
	{
	private:
		enum
		{
			Height = 4,
			Width = 4
		};

		typedef Detail::MatrixStaticBase<Height, Width, Real,
		Matrix<4, 4, Real> > Base;

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
		template <
			typename FirstExpression, 
			typename SecondExpression,
			typename ThirdExpression,
			typename FourthExpression>
		Matrix(
			const VectorExpression<4, Real, FirstExpression>& firstRow,
			const VectorExpression<4, Real, SecondExpression>& secondRow,
			const VectorExpression<4, Real, ThirdExpression>& thirdRow,
			const VectorExpression<4, Real, FourthExpression>& fourthRow)
			: Base()
		{
			set(firstRow, secondRow, thirdRow, fourthRow);
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
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m02;
			Base::data()[3] = m03;

			Base::data()[4] = m10;
			Base::data()[5] = m11;
			Base::data()[6] = m12;
			Base::data()[7] = m13;

			Base::data()[8] = m20;
			Base::data()[9] = m21;
			Base::data()[10] = m22;
			Base::data()[11] = m23;

			Base::data()[12] = m30;
			Base::data()[13] = m31;
			Base::data()[14] = m32;
			Base::data()[15] = m33;
		}

		Matrix& operator=(
			const Matrix& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		template <typename Expression>
		Matrix<Height, Width, Real>& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix<Height, Width, Real>&)Base::operator=(that);
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
			Base::data()[0] = m00;
			Base::data()[1] = m01;
			Base::data()[2] = m02;
			Base::data()[3] = m03;

			Base::data()[4] = m10;
			Base::data()[5] = m11;
			Base::data()[6] = m12;
			Base::data()[7] = m13;

			Base::data()[8] = m20;
			Base::data()[9] = m21;
			Base::data()[10] = m22;
			Base::data()[11] = m23;

			Base::data()[12] = m30;
			Base::data()[13] = m31;
			Base::data()[14] = m32;
			Base::data()[15] = m33;
		}

		template <
			typename FirstExpression, 
			typename SecondExpression,
			typename ThirdExpression,
			typename FourthExpression>
		void set(
			const VectorExpression<4, Real, FirstExpression>& firstRow,
			const VectorExpression<4, Real, SecondExpression>& secondRow,
			const VectorExpression<4, Real, ThirdExpression>& thirdRow,
			const VectorExpression<4, Real, FourthExpression>& fourthRow)
		{
			Base::data()[0] = firstRow[0];
			Base::data()[1] = firstRow[1];
			Base::data()[2] = firstRow[2];
			Base::data()[3] = firstRow[3];
			
			Base::data()[4] = secondRow[0];
			Base::data()[5] = secondRow[1];
			Base::data()[6] = secondRow[2];
			Base::data()[7] = secondRow[3];
			
			Base::data()[8] = thirdRow[0];
			Base::data()[9] = thirdRow[1];
			Base::data()[10] = thirdRow[2];
			Base::data()[11] = thirdRow[3];

			Base::data()[12] = fourthRow[0];
			Base::data()[13] = fourthRow[1];
			Base::data()[14] = fourthRow[2];
			Base::data()[15] = fourthRow[3];
		}
	};

}

#endif
