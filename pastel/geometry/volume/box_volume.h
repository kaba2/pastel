// Description: Volume of a box
// Documentation: volume.txt

#ifndef PASTELGEOMETRY_BOX_VOLUME_H
#define PASTELGEOMETRY_BOX_VOLUME_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	template <typename Real, int N>
	Real volume(const AlignedBox<Real, N>& box);

	template <typename Real, int N>
	Real volume(const Box<Real, N>& box);

	template <typename Real, int N>
	Real boxVolume(const Vector<Real, N>& width);

}

#include "pastel/geometry/volume/box_volume.hpp"

#endif
