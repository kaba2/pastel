// Description: Basic matrix expressions
// Detail: negation, addition, subtraction, multiplication etc.

#ifndef PASTEL_MATRIXEXPRESSION_H
#define PASTEL_MATRIXEXPRESSION_H

#include "pastel/math/matrix.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixNegation;

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixAddition;

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixSubtraction;

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixMultiplication;

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixScalarMultiplication;

	template <typename Real, int Height, int Width, typename Expression>
	class MatrixExpression
	{
	protected:
		MatrixExpression()
		{
		}

		MatrixExpression(const MatrixExpression& that)
		{
		}

		MatrixExpression& operator=(const MatrixExpression& that)
		{
			return *this;
		}

	public:
		Real operator()(integer y, integer x) const
		{
			return ((const Expression&)*this)(y, x);
		}

		integer width() const
		{
			return ((const Expression&)*this).width();
		}

		integer height() const
		{
			return ((const Expression&)*this).height();
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return ((const Expression&)*this).involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return ((const Expression&)*this).evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

		template <typename RightExpression>
		bool operator==(
			const MatrixExpression<Real, Height, Width, RightExpression>& right) const
		{
			const Expression& left = (const Expression&)*this;

			PENSURE_OP(left.width(), ==, right.width());
			PENSURE_OP(left.height(), ==, right.height());

			const integer leftWidth = width();
			const integer leftHeight = height();

			for (integer y = 0;y < leftHeight;++y)
			{
				for (integer x = 0;x < leftWidth;++x)
				{
					if (!(left(y, x) == right(y, x)))
					{
						return false;
					}
				}
			}

			return true;
		}

		template <typename RightExpression>
		bool operator!=(
			const MatrixExpression<Real, Height, Width, RightExpression>& right) const
		{
			return !(*this == right);
		}

		// Negation

		const MatrixNegation<Real, Height, Width, Expression> operator-() const
		{
			return MatrixNegation<Real, Height, Width, Expression>((const Expression&)*this);
		}

		// Summation

		template <typename RightExpression>
		const MatrixAddition<Real, Height, Width, Expression, 
			RightExpression>
			operator+(const MatrixExpression
			<Real, Height, Width, RightExpression>& right) const
		{
			return MatrixAddition
				<Real, Height, Width, Expression, 
				RightExpression >
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		// Subtraction

		template <typename RightExpression>
		const MatrixSubtraction<Real, Height, Width, Expression, 
			RightExpression>
			operator-(const MatrixExpression
			<Real, Height, Width, RightExpression>& right) const
		{
			return MatrixSubtraction
				<Real, Height, Width, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		// Multiplication

		template <int RightWidth, typename RightExpression>
		const MatrixMultiplication<Real, Height, RightWidth, Expression, 
			RightExpression>
			operator*(const MatrixExpression
			<Real, Width, RightWidth, RightExpression>& right) const
		{
			return MatrixMultiplication
				<Real, Height, RightWidth, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		const MatrixScalarMultiplication<Real, Height, Width, Expression>
			operator*(const Real& right) const
		{
			return MatrixScalarMultiplication
				<Real, Height, Width, Expression>
				((const Expression&)*this, right);
		}

		friend const MatrixScalarMultiplication<Real, Height, Width, Expression>
			operator*(const Real& left,
			const MatrixExpression& right)
		{
			// Scalar multiplication is commutative.
			return MatrixScalarMultiplication
				<Real, Height, Width, Expression>
				((const Expression&)right, left);
		}

		const MatrixScalarMultiplication<Real, Height, Width, Expression>
			operator/(const Real& right) const
		{
			return MatrixScalarMultiplication
				<Real, Height, Width, Expression>
				((const Expression&)*this, inverse(right));
		}

		// Division of a scalar by a matrix is not defined.
	};
	
	// Concrete expressions

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixNegation
		: public MatrixExpression<Real, Height, Width, 
		MatrixNegation<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixNegation& StorageType;

		explicit MatrixNegation(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return -data_(y, x);
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

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixAddition
		: public MatrixExpression<Real, Height, Width, 
		MatrixAddition<Real, Height, Width, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const MatrixAddition& StorageType;

		MatrixAddition(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.width(), ==, right.width());
			PENSURE_OP(left.height(), ==, right.height());
		}

		Real operator()(integer y, integer x) const
		{
			return left_(y, x) + right_(y, x);
		}

		integer width() const
		{
			return left_.width();
		}

		integer height() const
		{
			return left_.height();
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
			return left_.evaluateBeforeAssignment(memoryBegin, memoryEnd) ||
				right_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixSubtraction
		: public MatrixExpression<Real, Height, Width, 
		MatrixSubtraction<Real, Height, Width, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const MatrixSubtraction& StorageType;

		MatrixSubtraction(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.width(), ==, right.width());
			PENSURE_OP(left.height(), ==, right.height());
		}

		Real operator()(integer y, integer x) const
		{
			return left_(y, x) - right_(y, x);
		}

		integer width() const
		{
			return left_.width();
		}

		integer height() const
		{
			return left_.height();
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
			return left_.evaluateBeforeAssignment(memoryBegin, memoryEnd) ||
				right_.evaluateBeforeAssignment(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class MatrixMultiplication
		: public MatrixExpression<Real, Height, Width, 
		MatrixMultiplication<Real, Height, Width, 
		LeftExpression, RightExpression> >
	{
	public:
		typedef const MatrixMultiplication& StorageType;

		MatrixMultiplication(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.width(), ==, right.height());
		}

		Real operator()(integer y, integer x) const
		{
			Real sum = 0;
			const integer n = left_.width();

			for (integer i = 0;i < n;++i)
			{
				sum += left_(y, i) * right_(i, x);
			}

			return sum;
		}

		integer width() const
		{
			return right_.width();
		}

		integer height() const
		{
			return left_.height();
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
			// With multiplication, the involvement
			// of a given subexpression becomes
			// non-trivial. From now on we accept
			// any occurence.

			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <typename Real, int Height, int Width,
		typename Expression>
	class MatrixScalarMultiplication
		: public MatrixExpression<Real, Height, Width, 
		MatrixScalarMultiplication<Real, Height, Width, Expression> >
	{
	public:
		typedef const MatrixScalarMultiplication& StorageType;

		MatrixScalarMultiplication(
			const Expression& data,
			const Real& factor)
			: data_(data)
			, factor_(factor)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return data_(y, x) * factor_;
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
		const Real factor_;
	};

	template <typename Real, int Height, int Width>
	class MatrixDiagonal
		: public MatrixExpression<Real, Height, Width, 
		MatrixDiagonal<Real, Height, Width> >
	{
	public:
		// Since this expression contains data,
		// it must be stored by value.
		typedef const MatrixDiagonal StorageType;

		MatrixDiagonal(
			integer height,
			integer width,
			const Real& diagonal = 1)
			: width_(width)
			, height_(height)
			, diagonal_(diagonal)
		{
		}

		Real operator()(integer y, integer x) const
		{
			if (y == x)
			{
				return diagonal_;
			}

			return 0;
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
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return false;
		}

	private:
		const integer width_;
		const integer height_;
		const Real diagonal_;
	};

}

#endif
