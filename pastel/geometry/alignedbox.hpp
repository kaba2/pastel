#ifndef PASTELGEOMETRY_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_ALIGNEDBOX_HPP

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	void swap(AlignedBox<N, Real>& left,
		AlignedBox<N, Real>& right)
	{
		left.swap(right);
	}

}

#endif
