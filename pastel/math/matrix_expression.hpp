#ifndef PASTELMATH_MATRIX_EXPRESSION_HPP
#define PASTELMATH_MATRIX_EXPRESSION_HPP

#include "pastel/math/matrix_expression.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, typename Expression>
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
			const MatrixExpression<Real, RightExpression>& right) const
		{
			const Expression& left = (const Expression&)*this;

			PENSURE_OP(left.width(), ==, right.width());
			PENSURE_OP(left.height(), ==, right.height());

			integer leftWidth = width();
			integer leftHeight = height();

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
			const MatrixExpression<Real, RightExpression>& right) const
		{
			return !(*this == right);
		}

		// Negation

		MatrixNegation<Real, Expression> operator-() const
		{

			return MatrixNegation<Real, Expression>((const Expression&)*this);
		}

		// Summation

		template <typename RightExpression>
		MatrixAddition<Real, Expression, 
			RightExpression>
			operator+(const MatrixExpression

			<Real, RightExpression>& right) const
		{
			return MatrixAddition
				<Real, Expression, 
				RightExpression >
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		// Subtraction

		template <typename RightExpression>
		MatrixSubtraction<Real, Expression, 
			RightExpression>
			operator-(const MatrixExpression

			<Real, RightExpression>& right) const
		{
			return MatrixSubtraction
				<Real, Expression, 
				RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		// Multiplication

		template <typename RightExpression>
		MatrixMultiplication<Real, Expression, RightExpression>

			operator*(const MatrixExpression<Real, RightExpression>& right) const
		{
			return MatrixMultiplication<Real, Expression, RightExpression>
				((const Expression&)*this,
				(const RightExpression&)right);
		}

		MatrixScalarAddition<Real, Expression>
			operator+(Real right) const
		{
			return MatrixScalarAddition
				<Real, Expression>

				((const Expression&)*this, right);
		}

		friend const MatrixScalarAddition<Real, Expression>
			operator+(Real left, const MatrixExpression& right)
		{
			// Scalar addition is commutative.
			return MatrixScalarAddition
				<Real, Expression>
				((const Expression&)right, left);
		}

		MatrixScalarAddition<Real, Expression>
			operator-(Real right) const
		{
			return MatrixScalarAddition<Real, Expression>

				((const Expression&)*this, -right);
		}

		friend const MatrixScalarAddition<Real, Expression>
			operator-(Real left, const MatrixExpression& right)
		{
			// Scalar addition is commutative.
			return MatrixScalarAddition<Real, Expression>
				((const Expression&)right, -left);
		}

		MatrixScalarMultiplication<Real, Expression>

			operator*(Real right) const
		{
			return MatrixScalarMultiplication<Real, Expression>
				((const Expression&)*this, right);
		}

		friend const MatrixScalarMultiplication<Real, Expression>
			operator*(Real left, const MatrixExpression& right)
		{
			// Scalar multiplication is commutative.
			return MatrixScalarMultiplication<Real, Expression>
				((const Expression&)right, left);
		}

		MatrixScalarMultiplication<Real, Expression>
			operator/(Real right) const
		{
			return MatrixScalarMultiplication<Real, Expression>

				((const Expression&)*this, inverse(right));
		}

		// Division of a scalar by a matrix is not defined.
	};
	
	// Concrete expressions

	template <typename Real, typename Expression>
	class MatrixNegation
		: public MatrixExpression<Real, 
		MatrixNegation<Real, Expression> >
	{
	public:
		using StorageType = const MatrixNegation&;

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
		typename LeftExpression,
		typename RightExpression>
	class MatrixAddition
		: public MatrixExpression<Real, 
		MatrixAddition<Real, 
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const MatrixAddition&;

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
		typename LeftExpression,
		typename RightExpression>
	class MatrixSubtraction
		: public MatrixExpression<Real, 
		MatrixSubtraction<Real, 
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const MatrixSubtraction&;

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
		typename LeftExpression,
		typename RightExpression>
	class MatrixMultiplication
		: public MatrixExpression<Real, 
		MatrixMultiplication<Real, 
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const MatrixMultiplication&;

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
			integer n = left_.width();

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

	template <typename Real, typename Expression>
	class MatrixScalarMultiplication
		: public MatrixExpression<Real, 
		MatrixScalarMultiplication<Real, Expression> >
	{
	public:
		// Since this expression contains data,
		// it must be stored by value.
		using StorageType = const MatrixScalarMultiplication;

		MatrixScalarMultiplication(
			const Expression& data,
			Real factor)
			: data_(data)
			, factor_(std::move(factor))
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
		Real factor_;
	};

	template <typename Real, typename Expression>
	class MatrixScalarAddition
		: public MatrixExpression<Real, 
		MatrixScalarAddition<Real, Expression> >
	{
	public:
		// Since this expression contains data,
		// it must be stored by value.
		using StorageType = const MatrixScalarAddition;

		MatrixScalarAddition(
			const Expression& data,
			Real term)
			: data_(data)
			, term_(term)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return data_(y, x) + term_;
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
		Real term_;
	};

	template <typename Real, typename Expression>
	class MatrixDiagonal
		: public MatrixExpression<Real, MatrixDiagonal<Real, Expression>>
	{
	public:
		// Since this expression contains data,
		// it must be stored by value.
		using StorageType = const MatrixDiagonal;

		explicit MatrixDiagonal(
			integer m, integer n,

			const Expression& diagonal)
			: m_(m)
			, n_(n)
			, diagonal_(diagonal)
		{
		}

		Real operator()(integer y, integer x) const
		{
			if (y == x && x >= 0 && x < diagonal_.size())
			{
				return diagonal_[x];
			}

			return 0;
		}

		integer width() const
		{
			return n_;
		}

		integer height() const
		{
			return m_;
		}

		bool involves(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return diagonal_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin, const void* memoryEnd) const
		{
			return diagonal_.involves(memoryBegin, memoryEnd);
		}

	private:
		integer m_;
		integer n_;
		typename Expression::StorageType diagonal_;
	};

	template <typename Real>
	class MatrixConstant
		: public MatrixExpression<Real, 
		MatrixConstant<Real> >
	{
	public:
		// Since this expression contains data,
		// it must be stored by value.
		using StorageType = const MatrixConstant;

		MatrixConstant(
			integer height,
			integer width,
			Real value = 0)
			: width_(width)
			, height_(height)
			, value_(std::move(value))
		{
			PENSURE_OP(height, >=, 0);
			PENSURE_OP(width, >=, 0);
		}

		Real operator()(integer y, integer x) const
		{
			return value_;
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
		integer width_;
		integer height_;
		Real value_;
	};

	template <
		typename Real,
		typename Expression>
	class MatrixTranspose
		: public MatrixExpression<Real,
		MatrixTranspose<Real, Expression> >
	{
	public:

		using StorageType = const MatrixTranspose&;

		explicit MatrixTranspose(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return data_(x, y);
		}

		integer width() const
		{
			return data_.height();
		}

		integer height() const
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
			// With transpose the involvement
			// of a subexpression becomes non-trivial,
			// so any occurence will do.

			return data_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <typename Real>
	class ArrayMatrix
		: public MatrixExpression<Real, ArrayMatrix<Real>>
	{
	public:
		using StorageType = const ArrayMatrix&;

		ArrayMatrix(
			integer height, 
			integer width,
			const Real* data)
			: data_(data)
			, width_(width)
			, height_(height)
		{
		}

		Real operator()(integer y, integer x) const
		{
			PENSURE_OP(x, >=, 0);
			PENSURE_OP(x, <, width_);
			PENSURE_OP(y, >=, 0);
			PENSURE_OP(y, <, height_);
			return data_[(y * width_) + x];
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
			return memoryOverlaps(
				data_, data_ + height_ * width_, 
				memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return memoryOverlaps(
				data_, data_ + height_ * width_, 
				memoryBegin, memoryEnd);
		}

	private:
		const Real* data_;
		integer width_;
		integer height_;
	};

	// Matrices vs vectors

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication
		: public VectorExpression<Real, N, MatrixVectorMultiplication<Real, N,
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const MatrixVectorMultiplication&;

		MatrixVectorMultiplication(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.width(), ==, right.size());
		}

		Real operator[](integer index) const
		{
			integer n = left_.width();
			
			Real sum = 0;
			for (integer x = 0;x < n;++x)
			{

				sum += left_(index, x) * right_[x];
			}

			return sum;
		}

		integer size() const
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
			// This is a non-trivial expression.
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class VectorMatrixMultiplication
		: public VectorExpression<Real, N, VectorMatrixMultiplication<Real, N, 
		LeftExpression, RightExpression> >
	{
	public:
		using StorageType = const VectorMatrixMultiplication&;

		VectorMatrixMultiplication(
			const LeftExpression& left,
			const RightExpression& right)
			: left_(left)
			, right_(right)
		{
			PENSURE_OP(left.size(), ==, right.height());
		}

		Real operator[](integer index) const
		{
			integer n = right_.height();
			
			Real sum = 0;
			for (integer y = 0;y < n;++y)
			{

				sum += left_[y] * right_(y, index);
			}

			return sum;
		}

		integer size() const
		{
			return right_.width();
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
			// This is a non-trivial expression.
			return left_.involves(memoryBegin, memoryEnd) ||
				right_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename LeftExpression::StorageType left_;
		typename RightExpression::StorageType right_;
	};

}

#endif
