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
	class Matrix<Dynamic, Dynamic, Real>
		: public MatrixExpression<Dynamic, Dynamic, Real, 
		Matrix<Dynamic, Dynamic, Real> >
	{
	public:
		typedef const Matrix& StorageType;

		typedef Real* Iterator;
		typedef const Real* ConstIterator;
		typedef VectorView<Dynamic, Real> Row;
		typedef ConstVectorView<Dynamic, Real> ConstRow;
		typedef typename Array<2, Real>::RowIterator RowIterator;
		typedef typename Array<2, Real>::ConstRowIterator ConstRowIterator;
		typedef typename Array<2, Real>::RowIterator ColumnIterator;
		typedef typename Array<2, Real>::ConstRowIterator ConstColumnIterator;

		typedef Pastel::View<2, Real, ArrayView<2, Array<2, Real> > > View;
		typedef Pastel::ConstView<2, Real, ConstArrayView<2, Array<2, Real> > > ConstView;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Matrix()
			: data_()
		{
		}

		template <int Height, int Width, typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: data_(that.width(), that.height())
		{
			*this = that;
		}

		Matrix(integer height, integer width)
			: data_(width, height, 0)
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
			: data_(width, height, dataAlias)
		{
		}

		Matrix& operator=(
			const Matrix& that)
		{
			data_ = that.data_;

			return *this;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				data_.data(), data_.data() + size());
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

			return data_(i);
		}

		const Real& operator()(integer i) const
		{
			PENSURE2(i >= 0 && i < size(), i, size());

			return data_(i);
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
				
				(*this) = Matrix(right);
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

			return *this;
		}

		template <int N, typename RightExpression>
		Matrix& operator*=(
			const MatrixExpression<N, N, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.height(), width(), right.height());

			Matrix& left = 
				*this;

			left = left * right;

			return left;
		}

		template <int Height, int Width, typename RightExpression>
		Matrix& operator+=(
			const MatrixExpression<Height, Width, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.involvesNonTrivially(&*begin(), &*end()))
			{
				*this += Matrix(right);
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

			return *this;
		}

		template <int Height, int Width, typename RightExpression>
		Matrix& operator-=(
			const MatrixExpression<Height, Width, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.involvesNonTrivially(&*begin(), &*end()))
			{
				*this -= Matrix(right);
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

			return *this;
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

			data_.setExtent(newWidth, newHeight, 0);

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
			data_.set(that);
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
		
		const Array<2, Real>& asArray() const
		{
			return data_;
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

	private:
		Array<2, Real> data_;
	};

}

#endif
