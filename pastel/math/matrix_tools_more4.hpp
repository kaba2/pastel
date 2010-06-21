#ifndef PASTEL_MATRIX_TOOLS_MORE4_HPP
#define PASTEL_MATRIX_TOOLS_MORE4_HPP

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixSum
		: public VectorExpression<Real, Width, MatrixSum<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixSum& StorageType;

		explicit MatrixSum(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real sum = 0;
			const integer n = data_.height();
			
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

		bool involvesNonTrivially(
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

	template <typename Real, int Height, int Width, typename Expression>
	MatrixSum<Real, Height, Width, Expression> sum(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		return MatrixSum<Real, Height, Width, Expression>(
			(const Expression&)that);
	}

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixMin
		: public VectorExpression<Real, Width, MatrixMin<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixMin& StorageType;

		explicit MatrixMin(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real minValue = infinity<Real>();
			const integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				const Real value = data_(i, index);
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

		bool involvesNonTrivially(
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

	template <typename Real, int Height, int Width, typename Expression>
	MatrixMin<Real, Height, Width, Expression> min(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		return MatrixMin<Real, Height, Width, Expression>(
			(const Expression&)that);
	}

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixMax
		: public VectorExpression<Real, Width, MatrixMax<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixMax& StorageType;

		explicit MatrixMax(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real maxValue = -infinity<Real>();
			const integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				const Real value = data_(i, index);
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

		bool involvesNonTrivially(
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

	template <typename Real, int Height, int Width, typename Expression>
	MatrixMax<Real, Height, Width, Expression> max(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		return MatrixMax<Real, Height, Width, Expression>(
			(const Expression&)that);
	}

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixAbs
		: public MatrixExpression<Real, Height, Width, 
		MatrixAbs<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixAbs& StorageType;

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

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real, int Height, int Width, typename Expression>
	MatrixAbs<Real, Height, Width, Expression> abs(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		return MatrixAbs<Real, Height, Width, Expression>(
			(const Expression&)that);
	}

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixRepeat
		: public MatrixExpression<Real, Height, Width, 
		MatrixRepeat<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixRepeat StorageType;

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

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return data_.involvesNonTrivially(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
		const integer dataWidth_;
		const integer dataHeight_;
		const integer width_;
		const integer height_;
	};

	template <typename Real, int Height, int Width, typename Expression>
	MatrixRepeat<Real, Height, Width, Expression> repeat(
		const MatrixExpression<Real, Height, Width, Expression>& that,
		integer yBlocks, integer xBlocks)
	{
		return MatrixRepeat<Real, Height, Width, Expression>(
			(const Expression&)that, yBlocks, xBlocks);
	}


	template <
		typename Real,
		typename Input_ConstView>
	class ConstViewMatrix
		: public MatrixExpression<Real, Dynamic, Dynamic, 
		ConstViewMatrix<Real, Input_ConstView> >
	{
	public:
		typedef const ConstViewMatrix StorageType;

		explicit ConstViewMatrix(
			const ConstView<2, Real, Input_ConstView>& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return data_(x, y);
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			// TODO: FIX:
			// It is possible that the
			// view is from a matrix 
			// in which case we should 
			// have a check for the views
			// also!

			return false;
		}

		bool involvesNonTrivially(
			const void* memoryBegin, const void* memoryEnd) const
		{
			// TODO: FIX:
			// It is possible that the
			// view is from a matrix 
			// in which case we should 
			// have a check for the views
			// also!

			return false;
		}

	private:
		const ConstView<2, Real, Input_ConstView> data_;
	};

	template <typename Real, typename Input_ConstView>
	ConstViewMatrix<Real, Input_ConstView> asMatrix(
		const ConstView<2, Real, Input_ConstView>& that)
	{
		return ConstViewMatrix<Real, Input_ConstView>(that);
	}

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class OuterProduct
		: public MatrixExpression<Real, Height, Width, 
		OuterProduct<Real, Height, Width, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const OuterProduct& StorageType;

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

		bool involvesNonTrivially(
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

	template <typename Real,int Height, int Width, 
		typename LeftExpression,
		typename RightExpression>
		OuterProduct<Real, Height, Width, LeftExpression, RightExpression>
		outerProduct(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right)
	{
		return OuterProduct<Real, Height, Width, LeftExpression, RightExpression>(
			(const LeftExpression&)left, (const RightExpression&)right);
	}

	template <typename Real, int N, typename Expression>
		OuterProduct<Real, N, N, Expression, Expression>
		outerProduct(
		const VectorExpression<Real, N, Expression>& that)
	{
		return Pastel::outerProduct(that, that);
	}

}

#endif
