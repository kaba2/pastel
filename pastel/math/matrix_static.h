// Description: Matrix class
// Detail: Static-sized implementation of the Matrix class.

#ifndef PASTEL_MATRIX_STATIC_H
#define PASTEL_MATRIX_STATIC_H

#include "pastel/math/matrix.h"

#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/memory_overlaps.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	namespace Detail
	{

		template <int Height, int Width, typename Real, typename Derived>
		class MatrixStaticBase
			: public MatrixExpression<Height, Width, Real, Derived>
		{
		public:
			typedef const Derived& StorageType;

			typedef Real* Iterator;
			typedef const Real* ConstIterator;
			typedef VectorView<Width, Real> Row;
			typedef ConstVectorView<Width, Real> ConstRow;
			typedef Iterator RowIterator;
			typedef ConstIterator ConstRowIterator;
			typedef SparseIterator<Real*> ColumnIterator;
			typedef ConstSparseIterator<const Real*> ConstColumnIterator;

			MatrixStaticBase()
			{
				// Initialize to an identity matrix.
				*this = 0;
				
				const integer minExtent = std::min(width(), height());
				for (integer i = 0;i < minExtent;++i)
				{
					(*this)(i, i) = 1;
				}
			}

			template <typename Expression>
			MatrixStaticBase(const MatrixExpression<Height, Width, Real, Expression>& that)
			{
				*this = that;
			}

			// Used for concept checking.
			~MatrixStaticBase()
			{
				BOOST_STATIC_ASSERT(Height > 0 && Width > 0);
			}

			MatrixStaticBase& operator=(
				const MatrixStaticBase& that)
			{
				ENSURE_OP(width(), ==, that.width());
				ENSURE_OP(height(), ==, that.height());

				std::copy(that.data_, that.data_ + that.size(),
					data_);

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

			bool involves(const void* memoryBegin, const void* memoryEnd) const
			{
				return Pastel::memoryOverlaps(
					memoryBegin, memoryEnd,
					data_, data_ + size());
			}

			bool involvesNonTrivially(const void* memoryBegin, const void* memoryEnd) const
			{
				return false;
			}

			CommaFiller<Real, Iterator> operator|=(
				const Real& that)
			{
				return commaFiller<Real>(begin(), end(), that);
			}

			Real& operator()(integer i)
			{
				PENSURE2(i >= 0 && i < size(), i, size());

				return data_[i];
			}

			const Real& operator()(integer i) const
			{
				PENSURE2(i >= 0 && i < size(), i, size());

				return data_[i];
			}

			Real& operator()(integer y, integer x)
			{
				PENSURE2(y >= 0 && y < Height, y, Height);
				PENSURE2(x >= 0 && x < Width, x, Width);

				return data_[y * Width + x];
			}

			const Real& operator()(integer y, integer x) const
			{
				PENSURE2(y >= 0 && y < Height, y, Height);
				PENSURE2(x >= 0 && x < Width, x, Width);

				return data_[y * Width + x];
			}

			Row operator[](integer y)
			{
				PENSURE2(y >= 0 && y < Height, y, Height);

				return Row(
					data_ + y * Width, Width);
			}

			ConstRow operator[](integer y) const
			{
				PENSURE2(y >= 0 && y < Height, y, Height);

				return ConstRow(
					data_ + y * Width, Width);
			}

			template <typename RightExpression>
			Derived& operator=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				// We allow the size of the matrix to
				// change in assignment.

				if (right.involvesNonTrivially(&*begin(), &*end()) ||
					width() != right.width() ||
					height() != right.height())
				{
					// The right expression contains this matrix
					// as a subexpression. We thus need to evaluate
					// the expression first.
					
					((Derived&)*this) = Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter = right(i, j);
							++iter;
						}
					}
				}

				return (Derived&)*this;
			}

			template <typename RightExpression>
			Derived& operator*=(
				const MatrixExpression<Width, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.height(), width(), right.height());

				Derived& left = 
					(Derived&)*this;

				left = left * right;

				return left;
			}

			template <typename RightExpression>
			Derived& operator+=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.involvesNonTrivially(&*begin(), &*end()))
				{
					*this += Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter += right(i, j);
							++iter;
						}
					}
				}

				return (Derived&)*this;
			}

			template <typename RightExpression>
			Derived& operator-=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.involvesNonTrivially(&*begin(), &*end()))
				{
					*this -= Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter -= right(i, j);
							++iter;
						}
					}
				}

				return (Derived&)*this;
			}

			// Matrices vs scalars

			// Matrix += scalar and Matrix -= scalar are not
			// supported because of the possibly ambiguity:
			// it is not clear whether it should mean
			// "add / subtract element-wise" or
			// "add / subtract by multiples of identity matrix".
			// For *= and /= these interpretations are equivalent.

			// The parameter is deliberately taken by value because
			// a reference could be from this matrix.
			Derived& operator*=(
				const Real right)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					(*iter) *= right;
					++iter;
				}

				return (Derived&)*this;
			}

			// No need to take the parameter by value,
			// because we construct the inverse.
			Derived& operator/=(
				const Real& right)
			{
				return (*this) *= inverse(right);
			}

			// The parameter is deliberately taken by value because
			// a reference could be from this matrix.
			Derived& operator=(const Real that)
			{
				std::fill(data_, data_ + size(), that);
				return (Derived&)*this;
			}

			Iterator begin()
			{
				return data_;
			}

			ConstIterator begin() const
			{
				return data_;
			}

			Iterator end()
			{
				return data_ + Width * Height;
			}

			ConstIterator end() const
			{
				return data_ + Width * Height;
			}

			RowIterator rowBegin(integer y)
			{
				return data_ + y * Width;
			}

			ConstRowIterator rowBegin(integer y) const
			{
				return data_ + y * Width;
			}

			RowIterator rowEnd(integer y)
			{
				return data_ + (y + 1) * Width;
			}

			ConstRowIterator rowEnd(integer y) const
			{
				return data_ + (y + 1) * Width;
			}

			ColumnIterator columnBegin(integer x)
			{
				return ColumnIterator(
					data_ + x, Width);
			}

			ConstColumnIterator columnBegin(integer x) const
			{
				return ConstColumnIterator(
					data_ + x, Width);
			}

			ColumnIterator columnEnd(integer x)
			{
				return ColumnIterator(
					data_ + x + Height * Width, Width);
			}

			ConstColumnIterator columnEnd(integer x) const
			{
				return ConstColumnIterator(
					data_ + x + Height * Width, Width);
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
			ENSURE_OP(width, ==, Width);
			ENSURE_OP(height, ==, Height);
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
			ENSURE_OP(width, ==, Width);
			ENSURE_OP(height, ==, Height);
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
			ENSURE_OP(width, ==, Width);
			ENSURE_OP(height, ==, Height);
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
			Base::operator[](0) = firstRow;
			Base::operator[](1) = secondRow;
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01,
			const Real& m10, const Real& m11)
			: Base()
		{
			Base::operator()(0) = m00;
			Base::operator()(1) = m01;
			Base::operator()(2) = m10;
			Base::operator()(3) = m11;
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
			ENSURE_OP(width, ==, Width);
			ENSURE_OP(height, ==, Height);
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
			Base::operator[](0) = firstRow;
			Base::operator[](1) = secondRow;
			Base::operator[](2) = thirdRow;
		}

		//! Constructs with the given elements.
		Matrix(
			const Real& m00, const Real& m01, const Real& m02,
			const Real& m10, const Real& m11, const Real& m12,
			const Real& m20, const Real& m21, const Real& m22)
			: Base()
		{
			Base::operator()(0) = m00;
			Base::operator()(1) = m01;
			Base::operator()(2) = m02;
			
			Base::operator()(3) = m10;
			Base::operator()(4) = m11;
			Base::operator()(5) = m12;
			
			Base::operator()(6) = m20;
			Base::operator()(7) = m21;
			Base::operator()(8) = m22;
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
			ENSURE_OP(width, ==, Width);
			ENSURE_OP(height, ==, Height);
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
			Base::operator[](0) = firstRow;
			Base::operator[](1) = secondRow;
			Base::operator[](2) = thirdRow;
			Base::operator[](3) = fourthRow;
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
			Base::operator()(0) = m00;
			Base::operator()(1) = m01;
			Base::operator()(2) = m02;
			Base::operator()(3) = m03;

			Base::operator()(4) = m10;
			Base::operator()(5) = m11;
			Base::operator()(6) = m12;
			Base::operator()(7) = m13;

			Base::operator()(8) = m20;
			Base::operator()(9) = m21;
			Base::operator()(10) = m22;
			Base::operator()(11) = m23;

			Base::operator()(12) = m30;
			Base::operator()(13) = m31;
			Base::operator()(14) = m32;
			Base::operator()(15) = m33;
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

}

#endif
