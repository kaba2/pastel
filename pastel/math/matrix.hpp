#ifndef PASTEL_MATRIX_HPP
#define PASTEL_MATRIX_HPP

#include "pastel/math/matrix.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

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
		typedef const MatrixVectorMultiplication& StorageType;

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
			const integer n = left_.width();
			
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

		bool involvesNonTrivially(
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

	template <typename Real, int Height, int Width, 
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Real, Height, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Real, Height, Width, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right)
	{
		return MatrixVectorMultiplication<Real, Height, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

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
		typedef const VectorMatrixMultiplication& StorageType;

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
			const integer n = right_.height();
			
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

		bool involvesNonTrivially(
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

	template <typename Real, int Height, int Width, 
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Real, Width, LeftExpression, RightExpression> operator *(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const MatrixExpression<Real, Height, Width, RightExpression>& right)
	{
		return VectorMatrixMultiplication<Real, Width, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

	// Matrices vs points

	template <typename Real, int Height, int Width>
	Vector<Real, Height> operator*(
		const Matrix<Real, Height, Width>& left,
		const Vector<Real, Width>& right)
	{
		const integer width = left.width();
		const integer height = left.height();

		ENSURE2(width == right.size(), width, right.size());

		Vector<Real, Height> result(ofDimension(height));

		const integer width = left.width();
		const integer height = left.height();

		for (integer i = 0;i < height;++i)
		{
			result[i] = right[0] * data_[i][0];
			for (integer j = 1;j < width;++j)
			{
				result[i] += right[j] * data_[i][j];
			}
		}

		return result;
	}

	template <typename Real, int Height, int Width>
	Vector<Real, Width> operator *(
		const Vector<Real, Height>& left,
		const Matrix<Real, Height, Width>& right)
	{
		const integer width = right.width();
		const integer height = right.height();

		ENSURE2(height == left.size(), height, left.size());

		Vector<Real, Width> result(ofDimension(width));

		for (integer i = 0;i < width;++i)
		{
			result[i] = left[0] * right[0][i];
			for (integer j = 1;j < height;++j)
			{
				result[i] += left[j] * right[j][i];
			}
		}

		return result;
	}

	template <typename Real, int Height, int Width>
	void swap(
		Matrix<Real, Height, Width>& left,
		Matrix<Real, Height, Width>& right)
	{
		left.swap(right);
	}

}

#endif
