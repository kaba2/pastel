#ifndef PASTELMATH_MATRIX_HPP
#define PASTELMATH_MATRIX_HPP

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
	Matrix<Dynamic, Dynamic, Real>::Matrix(
		const Matrix& that, MatrixTransposeTag)
		: data_()
	{
		const integer width = that.width();
		const integer height = that.height();

		data_.setExtent(width, height);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				data_(y, x) = that(x, y);
			}
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
	void Matrix<Dynamic, Dynamic, Real>::setSize(integer height, integer width)
	{
		data_.setExtent(width, height);

		const integer minSize = std::min(width, height);

		for (integer i = 0;i < minSize;++i)
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
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator*=(
		const PASTEL_NO_DEDUCTION(Real)& that)
	{
		const integer nWidth = width();
		const integer nHeight = height();

		for (integer y = 0;y < nHeight;++y)
		{
			for (integer x = 0;x < nWidth;++x)
			{
				data_(x, y) *= that;
			}
		}

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator/=(
		const PASTEL_NO_DEDUCTION(Real)& that)
	{
		*this *= 1 / that;

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator+=(
		const Matrix& that)
	{
		const integer nWidth = width();
		const integer nHeight = height();

		ENSURE2(nWidth == that.width(), nWidth, that.width());
		ENSURE2(nHeight == that.height(), nHeight, that.height());

		for (integer y = 0;y < nHeight;++y)
		{
			for (integer x = 0;x < nWidth;++x)
			{
				data_(x, y) += that.data_(x, y);
			}
		}

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator-=(
		const Matrix& that)
	{
		const integer nWidth = width();
		const integer nHeight = height();

		ENSURE2(nWidth == that.width(), nWidth, that.width());
		ENSURE2(nHeight == that.height(), nHeight, that.height());

		for (integer y = 0;y < nHeight;++y)
		{
			for (integer x = 0;x < nWidth;++x)
			{
				data_(x, y) -= that.data_(x, y);
			}
		}

		return *this;
	}

	template <typename Real>
	Matrix<Dynamic, Dynamic, Real>&
		Matrix<Dynamic, Dynamic, Real>::operator*=(
		const Matrix& right)
	{
		const integer nWidth = width();
		const integer nHeight = height();

		ENSURE2(nWidth == right.height(), nWidth, right.height());
		ENSURE2(right.height() == right.width(), right.height(), right.width());

		Matrix& left = *this;
		Matrix copyLeft(left);

		for (integer i = 0;i < nHeight;++i)
		{
			for (integer j = 0;j < nWidth;++j)
			{
				left(i, j) = copyLeft(i, 0) * right(0, j);
				for (integer k = 1;k < nWidth;++k)
				{
					left(i, j) += copyLeft(i, k) * right(k, j);
				}
			}
		}

		return left;
	}

	// Binary operators

	// Matrices vs matrices

	template <int LeftHeight, int LeftWidth,
		int RightWidth, typename Real>
		Matrix<LeftHeight, RightWidth, Real> operator*(
		const Matrix<LeftHeight, LeftWidth, Real>& left,
		const Matrix<LeftWidth, RightWidth, Real>& right)
	{
		const integer leftWidth = left.width();
		const integer leftHeight = left.height();
		const integer rightWidth = right.width();
		const integer rightHeight = right.height();

		Matrix<LeftHeight, RightWidth, Real> result(leftHeight, rightWidth);

		for (integer i = 0;i < leftHeight;++i)
		{
			for (integer j = 0;j < rightWidth;++j)
			{
				Real value(0);
				for (integer k = 0;k < rightHeight;++k)
				{
					value += left[i][k] * right[k][j];
				}
				result[i][j] = value;
			}
		}

		return result;
	}

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator+(
		const Matrix<Height, Width, Real>& left,
		const Matrix<Height, Width, Real>& right)
	{
		Matrix<Height, Width, Real> result(left);
		result += right;
		return result;
	}

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator-(
		const Matrix<Height, Width, Real>& left,
		const Matrix<Height, Width, Real>& right)
	{
		Matrix<Height, Width, Real> result(left);
		result -= right;
		return result;
	}

	// Matrices vs vectors

	template <int Height, int Width, typename Real>
	Vector<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Vector<Width, Real>& right)
	{
		const integer width = left.width();
		const integer height = left.height();

		ENSURE2(width == right.size(), width, right.size());

		Vector<Height, Real> result(height);

		for (integer i = 0;i < height;++i)
		{
			result[i] = left[i][0] * right[0];
			for (integer j = 1;j < width;++j)
			{
				result[i] += left[i][j] * right[j];
			}
		}

		return result;
	}

	template <int Height, int Width, typename Real>
	Vector<Width, Real> operator *(
		const Vector<Height, Real>& left,
		const Matrix<Height, Width, Real>& right)
	{
		const integer width = right.width();
		const integer height = right.height();

		ENSURE2(height == left.size(), height, left.size());

		Vector<Width, Real> result(width);

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

	// Matrices vs points

	template <int Height, int Width, typename Real>
	Point<Height, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const Point<Width, Real>& right)
	{
		const integer width = left.width();
		const integer height = left.height();

		ENSURE2(width == right.size(), width, right.size());

		Point<Height, Real> result(height);

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

		Point<Width, Real> result(width);

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

	// Matrices and scalars

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator*(
		const Matrix<Height, Width, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		Matrix<Height, Width, Real> result(left);
		result *= right;
		return result;
	}

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator/(
		const Matrix<Height, Width, Real>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		Matrix<Height, Width, Real> result(left);
		result /= right;
		return result;
	}

	template <int Height, int Width, typename Real>
	Matrix<Height, Width, Real> operator*(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const Matrix<Height, Width, Real>& right)
	{
		Matrix<Height, Width, Real> result(right);
		result *= left;
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
