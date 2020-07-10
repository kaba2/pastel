// Description: Volume of a box
// Documentation: volume.txt

#ifndef PASTELGEOMETRY_BOX_VOLUME_H
#define PASTELGEOMETRY_BOX_VOLUME_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/box.h"
#include "pastel/sys/vector/vector.h"

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
