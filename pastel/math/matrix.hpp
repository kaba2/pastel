#ifndef PASTEL_MATRIX_HPP
#define PASTEL_MATRIX_HPP

#include "pastel/math/matrix.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	// Matrices vs vectors

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication
		: public VectorExpression<N, Real,
		MatrixVectorMultiplication<N, Real,
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

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Height, Real, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Height, Width, Real, LeftExpression>& left,
		const VectorExpression<Width, Real, RightExpression>& right)
	{
		return MatrixVectorMultiplication<Height, Real, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

	template <
		int N,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class VectorMatrixMultiplication
		: public VectorExpression<N, Real,
		VectorMatrixMultiplication<N, Real,
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

	template <int Height, int Width, typename Real,
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Width, Real, LeftExpression, RightExpression> operator *(
		const VectorExpression<Height, Real, LeftExpression>& left,
		const MatrixExpression<Height, Width, Real, RightExpression>& right)
	{
		return VectorMatrixMultiplication<Width, Real, LeftExpression, RightExpression>(
			(const LeftExpression&)left, 
			(const RightExpression&)right);
	}

	// Matrices vs points

	template <int Height, int Width, typename Real>
	Point<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Width, Real>& right)
	{
		const integer width = left.width();
		const integer height = left.height();

		ENSURE2(width == right.size(), width, right.size());

		Point<Height, Real> result(ofDimension(height));

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

	template <int Height, int Width, typename Real>
	Point<Width, Real> operator *(
		const Point<Height, Real>& left,
		const Matrix<Height, Width, Real>& right)
	{
		const integer width = right.width();
		const integer height = right.height();

		ENSURE2(height == left.size(), height, left.size());

		Point<Width, Real> result(ofDimension(width));

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

	template <int Height, int Width, typename Real>
	void swap(
		Matrix<Height, Width, Real>& left,
		Matrix<Height, Width, Real>& right)
	{
		left.swap(right);
	}

}

#endif
