// Description: Volume of a box
// Documentation: volume.txt

#ifndef PASTEL_BOX_VOLUME_H
#define PASTEL_BOX_VOLUME_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <typename Real, int N>
	Real volume(const AlignedBox<Real, N>& box);

	template <typename Real, int N>
	Real volume(const Box<Real, N>& box);

	template <typename Real, int N>
	Real boxVolume(const Vector<Real, N>& width);

}

#include "pastel/geometry/box_volume.hpp"

#endif
