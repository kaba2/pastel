#ifndef PASTELMATH_MATRIX_TOOLS_HPP
#define PASTELMATH_MATRIX_TOOLS_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"

#include <iostream>

namespace Pastel
{

	template <int Height, int Width, typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Matrix<Height, Width, Real>& m)
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
