// Description: Matrix class
// Detail: Dynamic-sized implementation of the Matrix class.

#ifndef PASTEL_MATRIX_DYNAMIC_H
#define PASTEL_MATRIX_DYNAMIC_H

#include "pastel/math/matrix.h"

#include "pastel/sys/array.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/memory_overlaps.h"

namespace Pastel
{

	template <typename Real>
	class Matrix<Real, Dynamic, Dynamic>
		: public MatrixExpression<Real, Dynamic, Dynamic, 
		Matrix<Real, Dynamic, Dynamic> >
	{
	public:
		typedef const Matrix& StorageType;

		typedef Real* Iterator;
		typedef const Real* ConstIterator;
		typedef Array_VectorExpression<Real, Dynamic> Row;
		typedef ConstArray_VectorExpression<Real, Dynamic> ConstRow;
		typedef typename Array<Real, 2>::RowIterator RowIterator;
		typedef typename Array<Real, 2>::ConstRowIterator ConstRowIterator;
		typedef typename Array<Real, 2>::RowIterator ColumnIterator;
		typedef typename Array<Real, 2>::ConstRowIterator ConstColumnIterator;

		typedef Pastel::View<2, Real, ArrayView<2, Array<Real, 2> > > View;
		typedef Pastel::ConstView<2, Real, ConstArrayView<2, Array<Real, 2> > > ConstView;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Matrix()
			: data_()
		{
		}

		template <int Height, int Width, typename Expression>
		Matrix(const MatrixExpression<Real, Height, Width, Expression>& that)
			: data_(Vector2i(that.width(), that.height()))
		{
			*this = that;
		}

		Matrix(integer height, integer width)
			: data_(Vector2i(width, height), 0)
		{
			const integer minSize = std::min(width, height);

			for (integer i = 0;i < minSize;++i)
			{
				data_(i, i) = 1;
			}
		}

		Matrix(
			integer height, integer width,
			const Alias<Real*>& dataAlias)
			: data_(Vector2i(width, height), dataAlias)
		{
		}

		Matrix& operator=(
			const Matrix& that)
		{
			if (width() != that.width() ||
				height() != that.height())
			{
				Matrix copy(that);
				swap(copy);
			}
			else
			{
				data_ = that.data_;
			}

			return *this;
		}

		bool valid(integer y, integer x) const
		{
			if (y >= 0 && y < height() &&
				x >= 0 && x < width())
			{
				return true;
			}

			return false;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				data_.rawBegin(), data_.rawEnd());
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

			return data_(i);
		}

		const Real& operator()(integer i) const
		{
			PENSURE2(i >= 0 && i < size(), i, size());

			return data_(i);
		}

		SubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max)
		{
			const SubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x())));

			return result;
		}

		ConstSubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max) const
		{
			const ConstSubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x())));

			return result;
		}

		SubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta)
		{
			const SubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x()),
				Vector2i(delta.y(), delta.x())));

			return result;
		}

		ConstSubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta) const
		{
			const ConstSubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x()),
				Vector2i(delta.y(), delta.x())));

			return result;
		}

		Real& operator()(integer y, integer x)
		{
			PENSURE2(y >= 0 && y < height(), y, height());
			PENSURE2(x >= 0 && x < width(), x, width());

			return data_(x, y);
		}

		const Real& operator()(integer y, integer x) const
		{
			PENSURE2(y >= 0 && y < height(), y, height());
			PENSURE2(x >= 0 && x < width(), x, width());

			return data_(x, y);
		}

		Row operator[](integer y)
		{
			PENSURE2(y >= 0 && y < height(), y, height());

			return Row(&data_(0, y), width());
		}

		ConstRow operator[](integer y) const
		{
			PENSURE2(y >= 0 && y < height(), y, height());

			return ConstRow(&data_(0, y), width());
		}

		template <int Height, int Width, typename RightExpression>
		Matrix& operator=(
			const MatrixExpression<Real, Height, Width, RightExpression>& right)
		{
			// We allow the size of the matrix to
			// change in assignment.

			if (width() != right.width() ||
				height() != right.height())
			{
				// The extents do no match.
				Matrix copy(right);
				swap(copy);
			}
			else if (right.evaluateBeforeAssignment(&*begin(), &*end()))
			{
				// The right expression contains this matrix
				// as a subexpression. We thus need to evaluate
				// the expression first.

				Matrix copy(right);

				(*this) = copy;
			}
			else
			{
				const integer m = height();
				const integer n = width();
				Iterator iter = begin();

				for (integer i = 0;i < m;++i)
				{
					for (integer j = 0;j < n;++j)
					{
						*iter = right(i, j);
						++iter;
					}
				}
			}

			return *this;
		}

		template <int N, typename RightExpression>
		Matrix& operator*=(
			const MatrixExpression<Real, N, N, RightExpression>& right)
		{
			PENSURE2(width() == right.height(), width(), right.height());

			Matrix& left = 
				*this;

			left = left * right;

			return left;
		}

		template <int Height, int Width, typename RightExpression>
		Matrix& operator+=(
			const MatrixExpression<Real, Height, Width, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(&*begin(), &*end()))
			{
				*this += Matrix(right);
			}
			else
			{
				const integer m = height();
				const integer n = width();
				Iterator iter = begin();

				for (integer i = 0;i < m;++i)
				{
					for (integer j = 0;j < n;++j)
					{
						*iter += right(i, j);
						++iter;
					}
				}
			}

			return *this;
		}

		template <int Height, int Width, typename RightExpression>
		Matrix& operator-=(
			const MatrixExpression<Real, Height, Width, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(&*begin(), &*end()))
			{
				*this -= Matrix(right);
			}
			else
			{
				const integer m = height();
				const integer n = width();
				Iterator iter = begin();

				for (integer i = 0;i < m;++i)
				{
					for (integer j = 0;j < n;++j)
					{
						*iter -= right(i, j);
						++iter;
					}
				}
			}

			return *this;
		}

		// Matrices vs scalars

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		Matrix& operator+=(
			const Real right)
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				(*iter) += right;
				++iter;
			}

			return *this;
		}

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		Matrix& operator-=(
			const Real right)
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				(*iter) -= right;
				++iter;
			}

			return *this;
		}

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		Matrix& operator*=(
			const Real right)
		{
			Iterator iter = begin();
			const Iterator iterEnd = end();

			while(iter != iterEnd)
			{
				(*iter) *= right;
				++iter;
			}

			return *this;
		}

		// No need to take the parameter by value,
		// because we construct the inverse.
		Matrix& operator/=(
			const Real& right)
		{
			return (*this) *= inverse(right);
		}

		void swap(Matrix& that)
		{
			data_.swap(that.data_);
		}

		void clear()
		{
			data_.clear();
		}

		void setSize(integer newHeight, integer newWidth)
		{
			const integer oldMinSize = std::min(width(), height());

			data_.setExtent(Vector2i(newWidth, newHeight), 0);

			const integer newMinSize = std::min(newWidth, newHeight);

			for (integer i = oldMinSize;i < newMinSize;++i)
			{
				data_(i, i) = 1;
			}
		}
		
		integer width() const
		{
			return data_.width();
		}

		integer height() const
		{
			return data_.height();
		}

		integer size() const
		{
			return data_.size();
		}

		void set(const Real& that)
		{
			data_ = that;
		}

		void reshape(integer height, integer width)
		{
			data_.reshape(Vector2i(width, height));
		}

		View view()
		{
			return arrayView(data_);
		}

		ConstView constView() const
		{
			return constArrayView(data_);
		}
		
		const Array<Real>& asArray() const
		{
			return data_;
		}

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		Matrix& operator=(const Real that)
		{
			data_ = that;
			return *this;
		}

		Iterator begin()
		{
			return data_.begin();
		}

		ConstIterator begin() const
		{
			return data_.begin();
		}

		Iterator end()
		{
			return data_.end();
		}

		ConstIterator end() const
		{
			return data_.end();
		}

		RowIterator rowBegin(integer y)
		{
			return data_.rowBegin(y);
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return data_.rowBegin(y);
		}

		RowIterator rowEnd(integer y)
		{
			return data_.rowEnd(y);
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return data_.rowEnd(y);
		}

		ColumnIterator columnBegin(integer x)
		{
			return data_.columnBegin(x);
		}

		ConstColumnIterator columnBegin(integer x) const
		{
			return data_.columnBegin(x);
		}

		ColumnIterator columnEnd(integer x)
		{
			return data_.columnEnd(x);
		}

		ConstColumnIterator columnEnd(integer x) const
		{
			return data_.columnEnd(x);
		}

		Real* rawBegin()
		{
			return data_.rawBegin();
		}

		const Real* rawBegin() const
		{
			return data_.rawBegin();
		}

		Real* rawEnd()
		{
			return data_.rawEnd();
		}

		const Real* rawEnd() const
		{
			return data_.rawEnd();
		}

	private:
		Array<Real> data_;
	};

}

#endif
