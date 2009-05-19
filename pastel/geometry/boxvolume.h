#ifndef PASTEL_BOXVOLUME_H
#define PASTEL_BOXVOLUME_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
	Real volume(const AlignedBox<N, Real>& box);

	template <int N, typename Real>
	Real volume(const Box<N, Real>& box);

	template <int N, typename Real>
	Real boxVolume(const Vector<N, Real>& width);

}

#include "pastel/geometry/boxvolume.hpp"

#endif
