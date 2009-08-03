#ifndef PASTEL_BOXVOLUME_HPP
#define PASTEL_BOXVOLUME_HPP

#include "pastel/geometry/boxvolume.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real volume(const AlignedBox<Real, N>& box)
	{
		return boxVolume(box.max() - box.min());
	}

	template <int N, typename Real>
	Real volume(const Box<Real, N>& box)
	{
		return boxVolume(2 * box.width());
	}

	template <int N, typename Real>
	Real boxVolume(const Vector<Real, N>& width)
	{
		return product(width);
	}

}

#endif
