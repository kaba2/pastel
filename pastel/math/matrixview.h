#ifndef PASTEL_MATRIXVIEW_H
#define PASTEL_MATRIXVIEW_H

#include "pastel/math/matrix.h"

#include "pastel/sys/array.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/range.h"

#include "pastel/math/integer_tools.h"

namespace Pastel
{

	/*
	template <typename Real>
	class MatrixRange
	{
	public:
		MatrixRange(
			Real* data,
			integer height
			integer width,
			integer dy,
			integer dx)
			: data_(data)
			, height_(height)
			, width_(width)
			, dy_(dy)
			, dx_(dx)
		{
		}

		Real* data() const
		{
			return data_;
		}

		integer width() const
		{
			return width_;
		}

		integer height() const
		{
			return height_;
		}

		integer dx() const
		{
			return dx_;
		}

		integer dy() const
		{
			return dy_;
		}

	private:
		Real* data_;
		integer height_;
		integer width_;
		integer dy_;
		integer dx_;
	};
	*/

	template <typename Real>
	class MatrixView
		: public MatrixExpression<Dynamic, Dynamic, Real, 
		MatrixView<Real> >
	{
	public:
		typedef const MatrixView& StorageType;

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

		MatrixView()
			: data_(0)
			, width_(0)
			, height_(0)
			, dx_(0)
			, dy_(0)
			, dataBegin_(0)
			, dataEnd_(0)
		{
		}

		MatrixView(
			Real* data,
			const Range& yRange,
			const Range& xRange)
			: data_(data)
			, width_(xRange.numbers())
			, height_(yRange.numbers())
			, dx_(xRange.step())
			, dy_(yRange.step() * xRange.size())
			, dataBegin_(0)
			, dataEnd_(0)
		{
			PENSURE_OP(width_, >, 0);
			PENSURE_OP(height_, >, 0);

			computeDataRange();
		}

		MatrixView(
			const MatrixView& that,
			const Range& yRange,
			const Range& xRange)
			: data_(that.address(xRange.begin(), yRange.begin()))
			, width_(xRange.numbers())
			, height_(yRange.numbers())
			, dx_(that.dx_ * xRange.step())
			, dy_(that.dy_ * yRange.step())
			, dataBegin_(0)
			, dataEnd_(0)
		{
			PENSURE_OP(width_, >, 0);
			PENSURE_OP(height_, >, 0);

			computeDataRange();
		}

		MatrixView& operator=(
			const MatrixView& that)
		{
			ENSURE_OP(width_, ==, that.width_);
			ENSURE_OP(height_, ==, that.height_);

			if (that.involvesNonTrivially(dataBegin_, dataEnd_))
			{
				// The right expression contains this matrix
				// as a subexpression. We thus need to evaluate
				// the expression first.
				
				(*this) = Matrix<Dynamic, Dynamic, Real>(that);
			}
			else
			{
				for (integer y = 0;y < height_;++y)
				{
					std::copy(
						that.rowBegin(y), that.rowEnd(y),
						rowBegin(y));
				}
			}

			return *this;
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				dataBegin_, dataEnd_);
		}

		bool involvesNonTrivially(const void* memoryBegin, const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				dataBegin_, dataEnd_);
		}

		Real& operator()(integer y, integer x)
		{
			PENSURE2(y >= 0 && y < height(), y, height());
			PENSURE2(x >= 0 && x < width(), x, width());

			return *address(x, y);
		}

		const Real& operator()(integer y, integer x) const
		{
			PENSURE2(y >= 0 && y < height(), y, height());
			PENSURE2(x >= 0 && x < width(), x, width());

			return *address(x, y);
		}

		MatrixView operator[](integer y)
		{
			PENSURE2(y >= 0 && y < height(), y, height());

			return MatrixView(data_, Range(y, y + 1), Range(0, width_));
		}

		const MatrixView operator[](integer y) const
		{
			PENSURE2(y >= 0 && y < height(), y, height());

			return MatrixView(data_, Range(0, width_), Range(y, y + 1));
		}

		template <int Height, int Width, typename RightExpression>
		MatrixView& operator=(
			const MatrixExpression<Height, Width, Real, RightExpression>& right)
		{
			ENSURE(width() == right.width() &&
				height() == right.height());

			if (right.involvesNonTrivially(dataBegin_, dataEnd_))
			{
				// The right expression contains this matrix
				// as a subexpression. We thus need to evaluate
				// the expression first.
				
				(*this) = Matrix<Dynamic, Dynamic, Real>(right);
			}
			else
			{
				const integer m = height();
				const integer n = width();

				for (integer i = 0;i < m;++i)
				{
					RowIterator iter = rowBegin(i);
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
		MatrixView& operator*=(
			const MatrixExpression<N, N, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.height(), width(), right.height());

			MatrixView& left = 
				*this;

			left = left * right;

			return left;
		}

		template <int Height, int Width, typename RightExpression>
		MatrixView& operator+=(
			const MatrixExpression<Height, Width, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.involvesNonTrivially(dataBegin_, dataEnd_))
			{
				*this += MatrixView(right);
			}
			else
			{
				const integer m = height();
				const integer n = width();

				for (integer i = 0;i < m;++i)
				{
					RowIterator iter = rowBegin(i);
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
		MatrixView& operator-=(
			const MatrixExpression<Height, Width, Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.involvesNonTrivially(dataBegin_, dataEnd_))
			{
				*this -= MatrixView(right);
			}
			else
			{
				const integer m = height();
				const integer n = width();

				for (integer i = 0;i < m;++i)
				{
					RowIterator iter = rowBegin(i);
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

		// MatrixView += scalar and MatrixView -= scalar are not
		// supported because of the possibly ambiguity:
		// it is not clear whether it should mean
		// "add / subtract element-wise" or
		// "add / subtract by multiples of identity matrix".
		// For *= and /= these interpretations are equivalent.

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		MatrixView& operator*=(
			const Real right)
		{
			for (integer y = 0;y < height_;++y)
			{
				RowIterator iter = rowBegin(y);
				const RowIterator iterEnd = rowEnd(y);

				while(iter != iterEnd)
				{
					(*iter) *= right;
					++iter;
				}
			}

			return *this;
		}

		// No need to take the parameter by value,
		// because we construct the inverse.
		MatrixView& operator/=(
			const Real& right)
		{
			return (*this) *= inverse(right);
		}

		void swap(MatrixView& that)
		{
			std::swap(data_, that.data_);
			std::swap(width_, that.width_);
			std::swap(height_, that.height_);
			std::swap(dx_, that.dx_);
			std::swap(dy_, that.dy_);
			std::swap(dataBegin_, that.dataBegin_);
			std::swap(dataEnd_, that.dataEnd_);
		}

		integer width() const
		{
			return width_;
		}

		integer height() const
		{
			return height_;
		}

		integer size() const
		{
			return width_ * height_;
		}

		void set(const Real& that)
		{
			for (integer y = 0;y < height_;++y)
			{
				std::fill(
					rowBegin(y), rowEnd(y), that);
			}
		}

		RowIterator rowBegin(integer y)
		{
			return RowIterator(
				address(0, y), dx_);
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return ConstRowIterator(
				address(0, y), dx_);
		}

		RowIterator rowEnd(integer y)
		{
			return RowIterator(
				address(width_, y), dx_);
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return ConstRowIterator(
				address(width_, y), dx_);
		}

		ColumnIterator columnBegin(integer x)
		{
			return ColumnIterator(
				address(x, 0), dy_);
		}

		ConstColumnIterator columnBegin(integer x) const
		{
			return ConstColumnIterator(
				address(x, 0), dy_);
		}

		ColumnIterator columnEnd(integer x)
		{
			return ColumnIterator(
				address(x, height_), dy_);
		}

		ConstColumnIterator columnEnd(integer x) const
		{
			return ConstColumnIterator(
				address(x, height_), dy_);
		}

	private:
		Real* address(integer x, integer y) const
		{
			return data_ + x * dx_ + y * dy_;
		}

		void computeDataRange()
		{
			dataBegin_ = data_;
			dataEnd_ = data_;

			if (dx_ < 0)
			{
				dataBegin_ += dx_ * (width_ - 1);
			}
			else
			{
				dataEnd_ += dx_ * (width_ - 1);
			}

			if (dy_ < 0)
			{
				dataBegin_ += dy_ * (height_ - 1);
			}
			else
			{
				dataEnd_ += dy_ * (height_ - 1);
			}
			++dataEnd_;
		}

		Real* data_;
		integer width_;
		integer height_;
		integer dx_;
		integer dy_;

		Real* dataBegin_;
		Real* dataEnd_;
	};

}

#endif
