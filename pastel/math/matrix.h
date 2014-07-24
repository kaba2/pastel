// Description: Matrix

#ifndef PASTELMATH_MATRIX_H
#define PASTELMATH_MATRIX_H

#include "pastel/math/matrix_expression.h"

#include "pastel/sys/array.h"
#include "pastel/sys/memory_overlaps.h"
#include "pastel/sys/sparse_iterator.h"

#include <boost/range/iterator_range.hpp>

namespace Pastel
{

	template <typename Real>
	class SubMatrix;

	template <typename Real>
	class ConstSubMatrix;

}

namespace Pastel
{

	template <typename Real>
	class Matrix
		: public MatrixExpression<Real, Matrix<Real>>
	{
	public:
		using StorageType = const Matrix&;

		// Iterators
		using Iterator = Real*;
		using ConstIterator = const Real*;

		using Cursor = typename Array<Real>::Cursor;
		using ConstCursor = typename Array<Real>::ConstCursor;

		using RowIterator = typename Array<Real>::RowIterator;
		using ConstRowIterator = typename Array<Real>::ConstRowIterator;

		using ColumnIterator = typename Array<Real>::RowIterator;
		using ConstColumnIterator = typename Array<Real>::ConstRowIterator;

		// Iterator ranges
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		using RowRange = boost::iterator_range<RowIterator>;
		using ConstRowRange = boost::iterator_range<ConstRowIterator>;

		using ColumnRange = boost::iterator_range<ColumnIterator>;
		using ConstColumnRange = boost::iterator_range<ConstColumnIterator>;

		// Rows
		using Row = Array_VectorExpression<Real, Dynamic>;
		using ConstRow = ConstArray_VectorExpression<Real, Dynamic>;

		//! Copy-constructs from another matrix.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong
		*/
		Matrix(const Matrix& that)
			: data_(that.data_)
		{
		}

		//! Move-constructs from another matrix.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Matrix(Matrix&& that)
			: data_()
		{
			swap(that);
		}

		//! Constructs from a matrix expression.
		/*!
		Time complexity: O(that.width() * that.height()) * Real=
		Exception safety: strong
		
		Note: implicit conversion allowed.
		*/
		template <typename Expression>
		Matrix(const MatrixExpression<Real, Expression>& that)
			: data_(Vector2i(that.width(), that.height()))
		{
			*this = that;
		}

		//! Constructs an mxn identity-matrix.
		/*!
		Time complexity: O(m * n) * (Real-Construct + Real=)
		Exception safety: strong
		*/
		Matrix(integer m, integer n)
			: data_(Vector2i(n, m), 0)
		{
			integer minSize = std::min(n, m);
			for (integer i = 0;i < minSize;++i)
			{
				data_(i, i) = 1;
			}
		}

		//! Contructs a matrix from a shared array.

		/*!
		The memory region is assumed to be managed by
		someone else; no attempt will be made to
		release the memory.

		Time complexity: O(1)
		Exception safety: strong
		*/
		Matrix(integer m, integer n,
			const Alias<Real*>& dataAlias)
			: data_(Vector2i(n, m), dataAlias)
		{
		}

		//! Swaps two matrices.
		void swap(Matrix& that)
		{
			data_.swap(that.data_);
		}

		//! Makes the matrix 0x0.
		void clear()
		{
			data_.clear();
		}

		//! Sets the size of the matrix.
		/*!
		Preconditions:
		newHeight >= 0
		newWidth >= 0

		Data will be preserved (new(i, j) == old(i, j)), 
		and extended by zeros except by ones at the 
		diagonal. 
		*/
		void setSize(
			integer newHeight, 
			integer newWidth)
		{
			ENSURE_OP(newHeight, >=, 0);
			ENSURE_OP(newWidth, >=, 0);

			integer oldMinSize = std::min(width(), height());

			data_.setExtent(Vector2i(newWidth, newHeight), 0);

			integer newMinSize = std::min(newWidth, newHeight);

			for (integer i = oldMinSize;i < newMinSize;++i)
			{
				data_(i, i) = 1;
			}
		}

		//! Returns whether the given index is inside the matrix.

		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		returns:
		i >= 0 && i < m() && 
		j >= 0 && j < n()
		*/
		bool valid(integer i, integer j) const
		{
			if (i >= 0 && i < m() &&
				j >= 0 && j < n())
			{
				return true;
			}

			return false;
		}

		//! Returns whether the given memory region overlaps with this matrix.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		This function is part of being a matrix expression.
		*/
		bool involves(
			const void* memoryBegin, 
			const void* memoryEnd) const
		{
			return Pastel::memoryOverlaps(
				memoryBegin, memoryEnd,
				data_.rawBegin(), data_.rawEnd());
		}

		//! Returns whether a memory region needs to be evaluated before assigning to this.
		/*! 
		Time complexity: O(1)
		Exception safety: nothrow

		This function is part of being a matrix expression.
		*/
		bool evaluateBeforeAssignment(
			const void* memoryBegin, 
			const void* memoryEnd) const
		{
			return false;
		}

		//! Returns the i:th linearized element of the matrix.
		/*!
		Preconditions:
		i >= 0
		i < size()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Real& operator()(integer i)
		{
			PENSURE2(i >= 0 && i < size(), i, size());

			return data_(i);
		}

		//! Returns the i:th linearized element of the matrix.
		/*!
		Preconditions:
		i >= 0
		i < size()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Real& operator()(integer i) const
		{
			PENSURE2(i >= 0 && i < size(), i, size());

			return data_(i);
		}

		SubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max)
		{
			SubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x())));

			return result;
		}

		ConstSubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max) const
		{
			ConstSubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x())));

			return result;
		}

		SubMatrix<Real> operator()(
			const Vector2i& min,
			const Vector2i& max,
			const Vector2i& delta)
		{
			SubMatrix<Real> result(
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
			ConstSubMatrix<Real> result(
				data_(Vector2i(min.y(), min.x()),
				Vector2i(max.y(), max.x()),
				Vector2i(delta.y(), delta.x())));

			return result;
		}

		//! Returns the (i, j):th element of the matrix.

		/*!
		Preconditions:
		i >= 0 && i < height()
		j >= 0 && j < width()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Real& operator()(integer i, integer j)
		{
			PENSURE2(i >= 0 && i < height(), i, height());
			PENSURE2(j >= 0 && j < width(), j, width());

			return data_(j, i);
		}

		//! Returns the (i, j):th element of the matrix.
		/*!
		Preconditions:
		i >= 0 && i < height()
		j >= 0 && j < width()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Real& operator()(integer i, integer j) const
		{
			PENSURE2(i >= 0 && i < height(), i, height());
			PENSURE2(j >= 0 && j < width(), j, width());

			return data_(j, i);
		}

		//! Returns the i:th row of the matrix.
		/*!
		Preconditions:
		i >= 0 && i < height()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Row row(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, height());

			return Row(&data_(0, i), n(), data_.stride()[0]);
		}

		//! Returns the i:th row of the matrix.
		/*!
		Preconditions:
		i >= 0 && i < height()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstRow cRow(integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, height());

			return ConstRow(&data_(0, i), n(), data_.stride()[0]);
		}

		//! Returns the j:th column of the matrix.
		/*!
		Preconditions:
		j >= 0 && j < width()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Row column(integer j)
		{
			PENSURE_OP(j, >=, 0);
			PENSURE_OP(j, <, width());

			return Row(&data_(j, 0), m(), data_.stride()[1]);
		}

		//! Returns the j:th column of the matrix.
		/*!
		Preconditions:
		j >= 0 && j < width()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstRow cColumn(integer j) const
		{
			PENSURE_OP(j, >=, 0);
			PENSURE_OP(j, <, width());

			return ConstRow(&data_(j, 0), m(), data_.stride()[1]);
		}

		//! Copy-assigns from another matrix.
		/*!
		1) If the extents do not match, do copy-construct-swap.
		   Note: memory-address will be changed.
		2) Otherwise copy values. 
		   Note: memory-address will not be changed.

		Time complexity: O(that.size() + size())
		Exception safety: basic
		*/
		Matrix& operator=(const Matrix& that)
		{
			if (width() != that.width() ||
				height() != that.height())
			{
				// If the matrices differ in size, then we must
				// reallocate memory.
				Matrix copy(that);
				swap(copy);
			}
			else
			{
				// Otherwise we can assign the values
				// directly. This is the reason why 
				// we want to separate move-assignment
				// and copy-assignment.
				data_ = that.data_;
			}

			return *this;
		}

		//! Move-assigns from another matrix.
		/*!
		Memory-addresses will be changed.

		Time complexity: O(size())
		Exception safety: basic
		*/
		Matrix& operator=(Matrix&& that)
		{
			swap(that);
			that.clear();
			return *this;
		}

		//! Assigns from an initializer list.
		/*!
		The values are assigned in the order of the
		linearization. Extraneous elements will be ignored, 
		missing elements will not cause a modification.

		Time complexity: O(min(size(), that.size()))
		Exception safety: basic
		*/
		Matrix& operator=(const std::initializer_list<Real>& that)
		{
			integer n = std::min(size(), (integer)that.size());
			std::copy_n(that.begin(), n, begin());
			return *this;
		}

		//! Assigns from a matrix expression.
		/*!
		Time complexity: O(right.size()) * (Real-Construct + Real=)
		Exception safety: basic
		*/
		template <typename RightExpression>
		Matrix& operator=(
			const MatrixExpression<Real, RightExpression>& right)
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
				// We can copy the values directly.
				integer m = height();
				integer n = width();
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

		//! Multiplies with a matrix expression.
		template <typename RightExpression>
		Matrix& operator*=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.height(), width(), right.height());

			Matrix& left = 
				*this;

			left = left * right;

			return left;
		}

		//! Adds a matrix expression.
		template <typename RightExpression>
		Matrix& operator+=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(&*begin(), &*end()))
			{
				*this += Matrix(right);
			}
			else
			{
				integer m = height();
				integer n = width();
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

		//! Subtracts a matrix expression.
		template <typename RightExpression>
		Matrix& operator-=(
			const MatrixExpression<Real, RightExpression>& right)
		{
			PENSURE2(width() == right.width(), width(), right.width());
			PENSURE2(height() == right.height(), height(), right.height());

			if (right.evaluateBeforeAssignment(&*begin(), &*end()))
			{
				*this -= Matrix(right);
			}
			else
			{
				integer m = height();
				integer n = width();
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
		
		//! Sets all elements to a constant.
		Matrix& operator=(Real that)
		{
			set(that);
			return *this;
		}

		//! Adds a constant to all elements.
		Matrix& operator+=(Real right)
		{
			// Note: the parameter _must_ be a non-reference
			// because a reference could be from this matrix.

			Iterator iter = begin();
			Iterator iterEnd = end();
			while(iter != iterEnd)
			{

				(*iter) += right;
				++iter;
			}

			return *this;
		}

		//! Subtracts a constant from all elements.
		Matrix& operator-=(Real right)
		{
			// Note: the parameter _must_ be a non-reference
			// because a reference could be from this matrix.

			Iterator iter = begin();
			Iterator iterEnd = end();
			while(iter != iterEnd)
			{

				(*iter) -= right;
				++iter;
			}

			return *this;
		}

		//! Multiplies all elements with a constant.
		Matrix& operator*=(Real right)
		{
			// Note: the parameter _must_ be a non-reference
			// because a reference could be from this matrix.

			Iterator iter = begin();
			Iterator iterEnd = end();
			while(iter != iterEnd)
			{

				(*iter) *= right;
				++iter;
			}

			return *this;
		}

		//! Divides all elements by a constant.
		Matrix& operator/=(Real right)
		{
			return (*this) *= inverse(right);
		}
		
		//! Returns the width of the matrix.
		integer width() const
		{
			return data_.width();
		}

		//! Returns the width of the matrix.
		integer n() const
		{
			return width();
		}

		//! Returns the height of the matrix.
		integer height() const
		{
			return data_.height();
		}

		//! Returns the height of the matrix.
		integer m() const
		{
			return height();
		}

		//! Returns the number of elements in the matrix.
		integer size() const
		{
			return data_.size();
		}

		//! Sets all elements to a constant.
		void set(Real that)
		{
			data_ = that;
		}

		//! Reshapes the matrix.
		/*!
		Reshaping means linearizing the matrix data, and then
		making it a matrix again row by row, but possibly with 
		different widths and heights than before.

		Preconditions:
		m * n == height() * width()
		*/
		void reshape(integer m, integer n)
		{
			PENSURE_OP(m, >=, 0);
			PENSURE_OP(n, >=, 0);
			PENSURE_OP(m * n, ==, height() * width());
			data_.reshape(Vector2i(n, m));
		}

		//! Returns the underlying array.
		const Array<Real>& asArray() const
		{
			return data_;
		}

		// Iterators

		Iterator begin()
		{
			return data_.begin();
		}

		ConstIterator cbegin() const
		{
			return data_.cbegin();
		}

		Iterator end()
		{
			return data_.end();
		}

		ConstIterator cend() const
		{
			return data_.cend();
		}

		Range range()
		{
			return data_.range();
		}

		ConstRange cRange() const
		{
			return data_.cRange();
		}

		// Row iterators

		RowIterator rowBegin(integer y)
		{
			return data_.rowBegin(y);
		}

		ConstRowIterator cRowBegin(integer y) const
		{
			return data_.cRowBegin(y);
		}

		RowIterator rowEnd(integer y)
		{
			return data_.rowEnd(y);
		}

		ConstRowIterator cRowEnd(integer y) const
		{
			return data_.cRowEnd(y);
		}

		RowRange rowRange(integer y)
		{
			return data_.rowRange(y);
		}

		ConstRowRange cRowRange(integer y) const
		{
			return data_.cRowRange(y);
		}

		// Column iterators

		ColumnIterator columnBegin(integer x)
		{
			return data_.columnBegin(x);
		}

		ConstColumnIterator cColumnBegin(integer x) const
		{
			return data_.cColumnBegin(x);
		}

		ColumnIterator columnEnd(integer x)
		{
			return data_.columnEnd(x);
		}

		ConstColumnIterator cColumnEnd(integer x) const
		{
			return data_.cColumnEnd(x);
		}

		ColumnRange columnRange(integer y)
		{
			return data_.columnRange(y);
		}

		ConstColumnRange cColumnRange(integer y) const
		{
			return data_.cColumnRange(y);
		}

		// Raw data

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
		// We will not allow to default-construct
		// a 0x0 matrix, because it easily leads 
		// to bugs caused by forgetting to specify 
		// the extents.
		Matrix() = delete;

		Array<Real> data_;
	};

	class SingularMatrix_Exception {};

}

#include "pastel/math/matrix_view.h"
#include "pastel/math/matrix_tools.h"

#endif
