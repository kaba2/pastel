#ifndef PASTELGEOMETRY_BOX_VOLUME_HPP
#define PASTELGEOMETRY_BOX_VOLUME_HPP

#include "pastel/geometry/volume/box_volume.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real volume(const AlignedBox<Real, N>& box)
	{
		return boxVolume(box.max() - box.min());
	}

	template <typename Real, int N>
	Real volume(const Box<Real, N>& box)
	{
		return boxVolume(2 * box.width());
	}

	template <typename Real, int N>
	Real boxVolume(const Vector<Real, N>& width)
	{
		return product(width);
	}

}

#endif
