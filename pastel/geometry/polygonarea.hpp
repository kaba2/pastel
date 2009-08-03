#ifndef PASTEL_POLYGONAREA_HPP
#define PASTEL_POLYGONAREA_HPP

#include "pastel/geometry/polygonarea.h"

namespace Pastel
{

	template <typename Real>
	Real signedArea(const Polygon<Real, 2>& polygon)
	{
		const integer n = polygon.size();

		if (n < 3)
		{
			return 0;
		}

		// This formula is essentially
		// an application of Green's theorem.

		Real result =
			polygon[0].x() *
			(polygon[1].y() - polygon[n - 1].y());

		for (integer i = 1;i < n - 1;++i)
		{
			result +=
				polygon[i].x() *
				(polygon[i + 1].y() - polygon[i - 1].y());
		}

		result +=
			polygon[n - 1].x() *
			(polygon[0].y() - polygon[n - 2].y());

		return result / 2;
	}

}

#endif
