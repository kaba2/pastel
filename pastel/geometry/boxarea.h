#ifndef PASTELGEOMETRY_BOXAREA_H
#define PASTELGEOMETRY_BOXAREA_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
	Real area(const AlignedBox<N, Real>& box);

	template <int N, typename Real>
	Real area(const Box<N, Real>& box);

	template <typename Real>
	Real boxArea(const Vector<1, Real>& width);

	template <typename Real>
	Real boxArea(const Vector<2, Real>& width);

	template <typename Real>
	Real boxArea(const Vector<3, Real>& width);

	template <typename Real>
	Real boxArea(const Vector<4, Real>& width);

	template <int N, typename Real>
	Real boxArea(const Vector<N, Real>& width);

}

#include "pastel/geometry/boxarea.hpp"

#endif
