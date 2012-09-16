#ifndef PASTELMATH_MATRIX_TOOLS_HPP
#define PASTELMATH_MATRIX_TOOLS_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple_tools.h"

namespace Pastel
{

	template <typename Real>
	Real& scalar(Matrix<Real, 1, 1>& matrix)
	{
		return matrix[0][0];
	}

	template <typename Real>
	const Real& scalar(const Matrix<Real, 1, 1>& matrix)
	{
		return matrix[0][0];
	}

	template <typename Real, int N, typename Expression>
	Vector<Real, N> diagonal(
		const MatrixExpression<Real, N, N, Expression>& matrix)
	{
		ENSURE_OP(matrix.width(), ==, matrix.height());

		const integer n = matrix.width();

		Vector<Real, N> result(ofDimension(n));

		for (int i = 0;i < n;++i)
		{
			result[i] = matrix(i, i);
		}

		return result;
	}

	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		ENSURE_OP(width, ==, height);

		matrix.set(0);

		for (integer i = 0;i < width;++i)
		{
			matrix(i, i) = value;
		}
	}

	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const Vector<Real, N>& values)
	{
		const integer size = values.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		matrix.set(0);

		for (integer i = 0;i < size;++i)
		{
			matrix(i, i) = values[i];
		}
	}

	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real, Height, Width>& matrix)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		using std::swap;

		for (integer y = 0;y < height;++y)
		{
			for (integer x = y + 1;x < width;++x)
			{
				swap(matrix(y, x), matrix(x, y));
			}
		}
	}

}

#include <iostream>

namespace Pastel
{

	template <int Height, int Width, typename Real, typename Expression>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixExpression<Real, Height, Width, Expression>& m)
	{
		const integer width = m.width();
		const integer height = m.height();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				stream << m(y, x) << ", ";
			}
			stream << std::endl;
		}

		return stream;
	}

}

#endif
