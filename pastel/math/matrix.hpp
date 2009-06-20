#ifndef PASTEL_MATRIX_HPP
#define PASTEL_MATRIX_HPP

#include "pastel/math/matrix.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>::Matrix()
		: data_()
	{
	}

	template <typename Real>
	template <int Height, int Width, typename Expression>
	Matrix<Dynamic, Dynamic, Real>::Matrix(
		const MatrixExpression<Height, Width, Real, Expression>& that)
		: data_(that.width(), that.height())
	{
		const integer leftWidth = width();
		const integer leftHeight = height();

		for (integer y = 0;y < leftHeight;++y)
		{
			for (integer x = 0;x < leftWidth;++x)
			{
				(*this)(y, x) = that(y, x);
			}
		}
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>::Matrix(integer height, integer width)
		: data_(width, height, 0)
	{
		const integer minSize = std::min(width, height);

		for (integer i = 0;i < minSize;++i)
		{
			data_(i, i) = 1;
		}
	}

	template <typename Real>
	void Matrix<Dynamic, Dynamic, Real>::swap(
		Matrix& that)
	{
		data_.swap(that.data_);
	}

	template <typename Real>
	void Matrix<Dynamic, Dynamic, Real>::clear()
	{
		data_.clear();
	}

	template <typename Real>
	void Matrix<Dynamic, Dynamic, Real>::setSize(integer newHeight, integer newWidth)
	{
		const integer oldMinSize = std::min(width(), height());

		data_.setExtent(newWidth, newHeight, 0);

		const integer newMinSize = std::min(newWidth, newHeight);

		for (integer i = oldMinSize;i < newMinSize;++i)
		{
			data_(i, i) = 1;
		}
	}

	template <typename Real>
	integer Matrix<Dynamic, Dynamic, Real>::width() const
	{
		return data_.width();
	}

	template <typename Real>
	integer Matrix<Dynamic, Dynamic, Real>::height() const
	{
		return data_.height();
	}

	template <typename Real>
	void Matrix<Dynamic, Dynamic, Real>::set(const Real& that)
	{
		data_.set(that);
	}

	template <typename Real>
	bool Matrix<Dynamic, Dynamic, Real>::involves(void* address) const
	{
		return this == address;
	}

	template <typename Real>
	bool Matrix<Dynamic, Dynamic, Real>::involvesNonTrivially(void* address) const
	{
		return false;
	}

	template <typename Real>
	Real& Matrix<Dynamic, Dynamic, Real>::operator()(integer y, integer x)
	{
		return data_(x, y);
	}

	template <typename Real>
	const Real& Matrix<Dynamic, Dynamic, Real>::operator()(integer y, integer x) const
	{
		return data_(x, y);
	}

	template <typename Real>
	TemporaryVector<Dynamic, Real> Matrix<Dynamic, Dynamic, Real>::operator[](integer y)
	{
		Vector<Dynamic, Real> result(
			ofDimension(width()), 
			withAliasing(&data_(0, y)));
		
		return result.asTemporary();
	}

	template <typename Real>
	const TemporaryVector<Dynamic, Real> Matrix<Dynamic, Dynamic, Real>::operator[](integer y) const
	{
		Vector<Dynamic, Real> result(
			ofDimension(width()), 
			withAliasing(const_cast<Real*>(&data_(0, y))));
		
		return result.asTemporary();
	}

	template <typename Real>
	template <typename Expression>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator=(
		const MatrixExpression<Dynamic, Dynamic, Real, Expression>& right)
	{
		if (right.involvesNonTrivially(this))
		{
			// The right expression contains this matrix
			// as a non-trivial subexpression. We thus need to evaluate
			// the expression first.
			
			Matrix<Dynamic, Dynamic, Real> copyRight(right);
			*this = copyRight;
		}
		else
		{
			// Else simply copy.

			Matrix<Dynamic, Dynamic, Real>& left = *this;

			const integer leftWidth = width();
			const integer leftHeight = height();

			for (integer i = 0;i < leftHeight;++i)
			{
				for (integer j = 0;j < leftWidth;++j)
				{
					left(i, j) = right(i, j);
				}
			}
		}

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator*=(
		const Real& that)
	{
		*this = *this * that;

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator/=(
		const Real& that)
	{
		*this = *this * inverse(that);

		return *this;
	}

	template <typename Real>
	template <typename Expression>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator+=(
		const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that)
	{
		*this = *this + that;

		return *this;
	}

	template <typename Real>
	template <typename Expression>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator-=(
		const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that)
	{
		*this = *this - that;

		return *this;
	}

	template <typename Real>
	template <typename Expression>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator*=(
		const MatrixExpression<Dynamic, Dynamic, Real, Expression>& that)
	{
		*this = *this * that;

		return *this;
	}

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
			PENSURE2(left.width() == right.size(), 
				left.width(), right.size());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// This is a non-trivial expression.
			return left_.involves(address) ||
				right_.involves(address);
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
			PENSURE2(left.size() == right.height(), 
				left.size(), right.height());
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
			void* address) const
		{
			return this == address ||
				left_.involves(address) ||
				right_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// This is a non-trivial expression.
			return left_.involves(address) ||
				right_.involves(address);
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
