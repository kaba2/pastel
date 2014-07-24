// Description: SubMatrix class

#ifndef PASTELMATH_MATRIXVIEW_H
#define PASTELMATH_MATRIXVIEW_H

#include "pastel/math/matrix.h"

#include "pastel/sys/array.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/sparse_iterator.h"
#include "pastel/sys/memory_overlaps.h"

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
	class ConstSubMatrix
		: public MatrixExpression<Real, ConstSubMatrix<Real> >
	{
	public:
		using StorageType = const ConstSubMatrix&;

		using ConstRow = ConstArray_VectorExpression<Real, Dynamic>;
		using ConstIterator = typename ConstSubArray<Real>::ConstIterator;
		using ConstRowIterator = typename ConstSubArray<Real>::ConstRowIterator;
		using ConstColumnIterator = typename ConstSubArray<Real>::ConstRowIterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		ConstSubMatrix()
			: data_()
		{
		}

		ConstSubMatrix(
			const Real* data,
			const Vector2i& stride,
			const Vector2i& extent)
			: data_(data, stride, extent)
		{
		}

		explicit ConstSubMatrix(
			const ConstSubArray<Real>& data)
			: data_(data)
		{
		}

		bool involves(const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		const Real& operator()(integer y, integer x) const
		{
			return data_(Vector2(x, y));
		}

		ConstSubMatrix operator()(
			const Vector2i& min,
			const Vector2i& max) const
		{
			ConstSubMatrix result(data_(min, max));
			return result;
		}

		ConstSubMatrix operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta) const
		{
			ConstSubMatrix result(data_(
				Vector2i(min.y(), min.x()), 
				Vector2i(max.y(), max.x()), 
				Vector2i(delta.y(), delta.x())));
			return result;
		}

		ConstSubMatrix operator[](integer y) const
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			ConstSubMatrix result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		ConstSubMatrix row(integer y) const
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			ConstSubMatrix result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		ConstSubMatrix column(integer x) const
		{

			PENSURE2(x >= 0 && x < width(), x, width());

			ConstSubMatrix result(
				data_(Vector2i(x, 0), Vector2i(x + 1, 0)));
			return result;
		}


		void swap(ConstSubMatrix& that)
		{
			data_.swap(that.data_);
		}

		integer width() const
		{
			return data_.extent()[0];
		}

		integer height() const
		{
			return data_.extent()[1];
		}

		integer size() const
		{
			return data_.size();
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return data_.rowBegin(0, Vector2i(0, y));
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return data_.rowEnd(0, Vector2i(0, y));
		}

		ConstColumnIterator columnBegin(integer x) const
		{
			return data_.columnBegin(1, Vector2i(x, 0));
		}

		ConstColumnIterator columnEnd(integer x) const
		{
			return data_.columnEnd(1, Vector2i(x, 0));
		}

	private:
		ConstSubMatrix& operator=(
			const ConstSubMatrix& that) = delete;

		ConstSubArray<Real> data_;
	};

	template <typename Real>
	class SubMatrix
		: public MatrixExpression<Real, SubMatrix<Real> >
	{
	public:
		using StorageType = const SubMatrix&;

		using Row = Array_VectorExpression<Real, Dynamic>;
		using ConstRow = ConstArray_VectorExpression<Real, Dynamic>;
		using Iterator = typename SubArray<Real>::Iterator;
		using ConstIterator = typename SubArray<Real>::ConstIterator;
		using RowIterator = typename SubArray<Real>::RowIterator;
		using ConstRowIterator = typename SubArray<Real>::ConstRowIterator;
		using ColumnIterator = typename SubArray<Real>::RowIterator;
		using ConstColumnIterator = typename SubArray<Real>::ConstRowIterator;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		SubMatrix()
			: data_()
		{
		}

		SubMatrix(
			Real* data,
			const Vector2i& stride,
			const Vector2i& extent)
			: data_(data, stride, extent)
		{
		}

		explicit SubMatrix(
			const SubArray<Real>& data)
			: data_(data)
		{
		}

		SubMatrix& operator=(
			const SubMatrix& that)
		{
			ENSURE_OP(width(), ==, that.width());
			ENSURE_OP(height(), ==, that.height());

			if (that.evaluateBeforeAssignment(data_.dataBegin(), data_.dataEnd()))
			{
				// The right expression contains this matrix
				// as a subexpression. We thus need to evaluate
				// the expression first.
				
				*this = Matrix<Real>(that);
			}
			else
			{
				integer m = height();
				for (integer y = 0;y < m;++y)
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
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(const void* memoryBegin, const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		Real& operator()(integer y, integer x)
		{
			return data_(Vector2i(x, y));
		}

		const Real& operator()(integer y, integer x) const
		{
			return data_(Vector2(x, y));
		}

		SubMatrix operator()(
			const Vector2i& min,
			const Vector2i& max)
		{
			SubMatrix result(data_(
				Vector2i(min.y(), min.x()), 
				Vector2i(max.y(), max.y())));
			return result;
		}

		ConstSubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max) const
		{
			ConstSubMatrix<Real> result(data_(
				Vector2i(min.y(), min.x()), 
				Vector2i(max.y(), max.y())));
			return result;
		}

		SubMatrix operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta)
		{
			SubMatrix result(data_(
				Vector2i(min.y(), min.x()), 
				Vector2i(max.y(), max.x()), 
				Vector2i(delta.y(), delta.x())));
			return result;
		}

		ConstSubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta) const
		{
			ConstSubMatrix<Real> result(data_(
				Vector2i(min.y(), min.x()), 
				Vector2i(max.y(), max.x()), 
				Vector2i(delta.y(), delta.x())));
			return result;
		}

		SubMatrix operator[](integer y)
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			SubMatrix result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		ConstSubMatrix<Real> operator[](integer y) const
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			ConstSubMatrix<Real> result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		SubMatrix row(integer y)
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			SubMatrix result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		ConstSubMatrix<Real> row(integer y) const
		{

			PENSURE2(y >= 0 && y < height(), y, height());

			ConstSubMatrix<Real> result(
				data_(Vector2i(0, y), Vector2i(0, y + 1)));
			return result;
		}

		SubMatrix column(integer x)
		{

			PENSURE2(x >= 0 && x < width(), x, width());

			SubMatrix result(
				data_(Vector2i(x, 0), Vector2i(x + 1, 0)));
			return result;
		}

		ConstSubMatrix<Real> column(integer x) const
		{

			PENSURE2(x >= 0 && x < width(), x, width());

			ConstSubMatrix<Real> result(
				data_(Vector2i(x, 0), Vector2i(x + 1, 0)));
			return result;
		}

		template <typename RightExpression>

		SubMatrix& operator=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			ENSURE(width() == right.width() &&
				height() == right.height());

			if (right.evaluateBeforeAssignment(data_.dataBegin(), data_.dataEnd()))
			{
				// The right expression contains this matrix
				// as a subexpression. We thus need to evaluate
				// the expression first.
				
				*this = Matrix<Real>(right);
			}
			else
			{
				integer m = height();
				integer n = width();

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

		template <typename RightExpression>
		SubMatrix& operator*=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.height(), width(), right.height());

			*this = *this * right;

			return *this;
		}

		template <typename RightExpression>
		SubMatrix& operator+=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(data_.dataBegin(), data_.dataEnd()))
			{
				*this += Matrix<Real>(right);
			}
			else
			{
				integer m = height();
				integer n = width();

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

		template <typename RightExpression>
		SubMatrix& operator-=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(data_.dataBegin(), data_.dataEnd()))
			{
				*this -= Matrix<Real>(right);
			}
			else
			{
				integer m = height();
				integer n = width();

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

		// SubMatrix += scalar and SubMatrix -= scalar are not
		// supported because of the possibly ambiguity:
		// it is not clear whether it should mean
		// "add / subtract element-wise" or
		// "add / subtract by multiples of identity matrix".
		// For *= and /= these interpretations are equivalent.

		// The parameter is deliberately taken by value because
		// a reference could be from this matrix.
		SubMatrix& operator*=(
			Real right)
		{
			integer m = height();

			for (integer y = 0;y < m;++y)
			{
				RowIterator iter = rowBegin(y);
				RowIterator iterEnd = rowEnd(y);

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
		SubMatrix& operator/=(
			const Real& right)
		{
			return (*this) *= inverse(right);
		}

		void swap(SubMatrix& that)
		{
			data_.swap(that.data_);
		}

		integer width() const
		{
			return data_.extent()[0];
		}

		integer height() const
		{
			return data_.extent()[1];
		}

		integer size() const
		{
			return data_.size();
		}

		RowIterator rowBegin(integer y)
		{
			return data_.rowBegin(0, Vector2i(0, y));
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return data_.rowBegin(0, Vector2i(0, y));
		}

		RowIterator rowEnd(integer y)
		{
			return data_.rowEnd(0, Vector2i(0, y));
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return data_.rowEnd(0, Vector2i(0, y));
		}

		ColumnIterator columnBegin(integer x)
		{
			return data_.columnBegin(1, Vector2i(x, 0));
		}

		ConstColumnIterator columnBegin(integer x) const
		{
			return data_.columnBegin(1, Vector2i(x, 0));
		}

		ColumnIterator columnEnd(integer x)
		{
			return data_.columnEnd(1, Vector2i(x, 0));
		}

		ConstColumnIterator columnEnd(integer x) const
		{
			return data_.columnEnd(1, Vector2i(x, 0));
		}

	private:
		SubArray<Real> data_;
	};

}

#endif
