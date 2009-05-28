#ifndef PASTEL_MATRIX_TOOLS_HPP
#define PASTEL_MATRIX_TOOLS_HPP

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple_tools.h"

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
			stream << m[y].asTuple() << std::endl;
		}

		return stream;
	}

}

#endif
