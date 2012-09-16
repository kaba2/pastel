#ifndef PASTELMATH_MATRIX_TOOLS_MORE2_HPP
#define PASTELMATH_MATRIX_TOOLS_MORE2_HPP

#include "pastel/math/matrix.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

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

}

#endif
