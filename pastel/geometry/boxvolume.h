// Description: Volume of a box
// Documentation: volume.txt

#ifndef PASTEL_BOXVOLUME_H
#define PASTEL_BOXVOLUME_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
	Real volume(const AlignedBox<Real, N>& box);

	template <int N, typename Real>
	Real volume(const Box<Real, N>& box);

	template <int N, typename Real>
	Real boxVolume(const Vector<Real, N>& width);

}

#include "pastel/geometry/boxvolume.hpp"

#endif
