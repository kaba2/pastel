#ifndef PASTEL_MATRIX_TOOLS_MORE2_HPP
#define PASTEL_MATRIX_TOOLS_MORE2_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/random.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <int Height, int Width, typename Real>
	void setRandomMatrix(
		Matrix<Height, Width, Real>& matrix)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		for (integer i = 0;i < height;++i)
		{
			for (integer j = 0;j < width;++j)
			{
				matrix(i, j) = random<Real>() * 2 - 1;
			}
		}
	}

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value)
	{
		const integer width = matrix.width();
		const integer height = matrix.height();

		ENSURE2(width == height, width, height);

		for (integer i = 0;i < width;++i)
		{
			matrix(i, i) = value;
		}
	}

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& values)
	{
		const integer size = values.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		for (integer i = 0;i < size;++i)
		{
			result(i, i) = values[i];
		}
	}

	// Householder

	template <int N, typename Real>
	void setHouseHolder(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& v)
	{
		const integer size = v.size();

		ENSURE2(matrix.width() == size, matrix.width(), size);
		ENSURE2(matrix.height() == size, matrix.height(), size);

		for (integer i = 0;i < size;++i)
		{
			for (integer j = 0;j < size;++j)
			{
				matrix(i, j) = v[j] * -2 * v[i];
			}

			matrix(i, i) += 1;
		}
	}

	template <int Height, int Width, typename Real>
	void setRandomRotation(
		Matrix<Height, Width, Real>& result)
	{
		const integer height = result.height();
		const integer width = result.width();

		ENSURE2(height <= width, height, width);

		std::vector<Vector<Width, Real> > orthonormalSet;
		orthonormalSet.reserve(height);

		orthonormalSet.push_back(
			randomVectorSphere<Width, Real>(width));
		result[0] = orthonormalSet.back();

		for (integer i = 1;i < height;++i)
		{
			orthonormalSet.push_back(
				perpendicular(width, orthonormalSet));
			result[i] = orthonormalSet.back();
		}
	}

}


#endif
