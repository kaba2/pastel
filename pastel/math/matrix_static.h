// Description: Matrix class
// Detail: Static-sized implementation of the Matrix class.

#ifndef PASTEL_MATRIX_STATIC_H
#define PASTEL_MATRIX_STATIC_H

#include "pastel/math/matrix.h"

#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/memory_overlaps.h"

namespace Pastel
{

	namespace Detail
	{

		template <typename Real, int Height, int Width, typename Derived>
		class MatrixStaticBase
			: public MatrixExpression<Real, Height, Width, Derived>
		{
		public:
			typedef const Derived& StorageType;

			typedef Real* Iterator;
			typedef const Real* ConstIterator;
			typedef Array_VectorExpression<Real, Width> Row;
			typedef ConstArray_VectorExpression<Real, Width> ConstRow;
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
			MatrixStaticBase(const MatrixExpression<Real, Height, Width, Expression>& that)
			{
				*this = that;
			}

			// Used for concept checking.
			~MatrixStaticBase()
			{
				PASTEL_STATIC_ASSERT(Height > 0 && Width > 0);
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

			bool valid(integer y, integer x) const
			{
				if (y >= 0 && y < Height &&
					x >= 0 && x < Width)
				{
					return true;
				}

				return false;
			}

			bool involves(const void* memoryBegin, const void* memoryEnd) const
			{
				return Pastel::memoryOverlaps(
					memoryBegin, memoryEnd,
					data_, data_ + size());
			}

			bool evaluateBeforeAssignment(const void* memoryBegin, const void* memoryEnd) const
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
				const MatrixExpression<Real, Height, Width, RightExpression>& right)
			{
				// We allow the size of the matrix to
				// change in assignment.

				if (right.evaluateBeforeAssignment(&*begin(), &*end()) ||
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
				const MatrixExpression<Real, Width, Width, RightExpression>& right)
			{
				PENSURE2(width() == right.height(), width(), right.height());

				Derived& left = 
					(Derived&)*this;

				left = left * right;

				return left;
			}

			template <typename RightExpression>
			Derived& operator+=(
				const MatrixExpression<Real, Height, Width, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.evaluateBeforeAssignment(&*begin(), &*end()))
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
				const MatrixExpression<Real, Height, Width, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.evaluateBeforeAssignment(&*begin(), &*end()))
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
					rawEnd() + x, Width);
			}

			ConstColumnIterator columnEnd(integer x) const
			{
				return ConstColumnIterator(
					rawEnd() + x, Width);
			}

			Real* rawBegin()
			{
				return data_;
			}

			const Real* rawBegin() const
			{
				return data_;
			}

			Real* rawEnd()
			{
				return data_ + Height * Width;
			}

			const Real* rawEnd() const
			{
				return data_ + Height * Width;
			}

		private:
			Real data_[Height * Width > 0 ? Height * Width : 1];
		};

	}

	template <typename Real, int Height = Dynamic, int Width = Height>
	class Matrix
		: public Detail::MatrixStaticBase<Real, Height, Width, 
		Matrix<Real, Height, Width> >
	{
	private:
		typedef Detail::MatrixStaticBase<Real, Height, Width, 
			Matrix<Real, Height, Width> > Base;

	public:
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
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
			const MatrixExpression<Real, Height, Width, Expression>& that)
		{
			return (Matrix<Real, Height, Width>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<Real, 1, 1>
		: public Detail::MatrixStaticBase<Real, 1, 1, 
		Matrix<Real, 1, 1> >
	{
	private:
		enum
		{
			Height = 1,
			Width = 1
		};

		typedef Detail::MatrixStaticBase<Real, Height, Width, 
			Matrix<Real, 1, 1> > Base;

	public:
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
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
			const VectorExpression<Real, 1, FirstExpression>& firstRow)
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
		Matrix<Real, Height, Width>& operator=(
			const MatrixExpression<Real, Height, Width, Expression>& that)
		{
			return (Matrix<Real, Height, Width>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<Real, 2, 2>
		: public Detail::MatrixStaticBase<Real, 2, 2, 
		Matrix<Real, 2, 2> >
	{
	private:
		enum
		{
			Height = 2,
			Width = 2
		};

		typedef Detail::MatrixStaticBase<Real, Height, Width, 
		Matrix<Real, 2, 2> > Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
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
			const VectorExpression<Real, 2, FirstExpression>& firstRow,
			const VectorExpression<Real, 2, SecondExpression>& secondRow)
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
		Matrix<Real, Height, Width>& operator=(
			const MatrixExpression<Real, Height, Width, Expression>& that)
		{
			return (Matrix<Real, Height, Width>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<Real, 3, 3>
		: public Detail::MatrixStaticBase<Real, 3, 3, 
		Matrix<Real, 3, 3> >
	{
	private:
		enum
		{
			Height = 3,
			Width = 3
		};

		typedef Detail::MatrixStaticBase<Real, Height, Width, 
		Matrix<Real, 3, 3> > Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
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
			const VectorExpression<Real, 3, FirstExpression>& firstRow,
			const VectorExpression<Real, 3, SecondExpression>& secondRow,
			const VectorExpression<Real, 3, ThirdExpression>& thirdRow)
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
		Matrix<Real, Height, Width>& operator=(
			const MatrixExpression<Real, Height, Width, Expression>& that)
		{
			return (Matrix<Real, Height, Width>&)Base::operator=(that);
		}
	};

	template <typename Real>
	class Matrix<Real, 4, 4>
		: public Detail::MatrixStaticBase<Real, 4, 4, 
		Matrix<Real, 4, 4> >
	{
	private:
		enum
		{
			Height = 4,
			Width = 4
		};

		typedef Detail::MatrixStaticBase<Real, Height, Width, 
		Matrix<Real, 4, 4> > Base;

	public:
		//! Constructs an identity matrix.
		Matrix()
			: Base()
		{
		}

		template <typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
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
			const VectorExpression<Real, 4, FirstExpression>& firstRow,
			const VectorExpression<Real, 4, SecondExpression>& secondRow,
			const VectorExpression<Real, 4, ThirdExpression>& thirdRow,
			const VectorExpression<Real, 4, FourthExpression>& fourthRow)
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
		Matrix<Real, Height, Width>& operator=(
			const MatrixExpression<Real, Height, Width, Expression>& that)
		{
			return (Matrix<Real, Height, Width>&)Base::operator=(that);
		}
	};

}

#endif
