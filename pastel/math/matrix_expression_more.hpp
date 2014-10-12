#ifndef PASTELMATH_MATRIX_EXPRESSION_MORE_HPP
#define PASTELMATH_MATRIX_EXPRESSION_MORE_HPP

#include "pastel/math/matrix_expression_more.h"

namespace Pastel
{

	template <
		typename Real,
		typename Expression>
	class MatrixSum
		: public VectorExpression<Real, Dynamic, MatrixSum<Real, Expression> >
	{
	public:
		using StorageType = const MatrixSum&;

		explicit MatrixSum(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real sum = 0;
			integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				sum += data_(i, index);
			}

			return sum;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, typename Expression>
	class MatrixMin
		: public VectorExpression<Real, Dynamic, MatrixMin<Real, Expression> >
	{
	public:
		using StorageType = const MatrixMin&;

		explicit MatrixMin(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real minValue = infinity<Real>();
			integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				Real value = data_(i, index);
				if (value < minValue)
				{
					minValue = value;
				}
			}

			return minValue;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, typename Expression>
	class MatrixMax
		: public VectorExpression<Real, Dynamic, MatrixMax<Real, Expression> >
	{
	public:
		using StorageType = const MatrixMax&;

		explicit MatrixMax(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real maxValue = -infinity<Real>();
			integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				Real value = data_(i, index);
				if (value > maxValue)
				{
					maxValue = value;
				}
			}

			return maxValue;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, typename Expression>
	class MatrixAbs
		: public MatrixExpression<Real, 
		MatrixAbs<Real, Expression> >
	{
	public:
		using StorageType = const MatrixAbs&;

		explicit MatrixAbs(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return mabs(data_(y, x));
		}

		integer width() const
		{
			return data_.width();
		}

		integer height() const
		{
			return data_.height();
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, typename Expression>
	class MatrixRepeat
		: public MatrixExpression<Real, 
		MatrixRepeat<Real, Expression> >
	{
	public:
		using StorageType = const MatrixRepeat;

		explicit MatrixRepeat(
			const Expression& data,
			integer yBlocks,
			integer xBlocks)
			: data_(data)
			, dataWidth_(data.width())
			, dataHeight_(data.height())
			, width_(data.width() * xBlocks)
			, height_(data.height() * yBlocks)
		{
			PENSURE_OP(xBlocks, >=, 0);
			PENSURE_OP(yBlocks, >=, 0);
		}

		Real operator()(integer y, integer x) const
		{
			return data_(y % dataHeight_, x % dataWidth_);
		}

		integer width() const
		{
			return width_;
		}

		integer height() const
		{
			return height_;
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
		integer dataWidth_;
		integer dataHeight_;
		integer width_;
		integer height_;
	};

	template <
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class OuterProduct
		: public MatrixExpression<Real, 
		OuterProduct<Real, 
		LeftExpression, RightExpression> >
	{
	public:

		using StorageType = const OuterProduct&;

		OuterProduct(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return left_[y] * right_[x];
		}

		integer width() const
		{
			return right_.size();
		}

		integer height() const
		{
			return left_.size();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			// This is a non-trivial combination.
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

}

#endif
