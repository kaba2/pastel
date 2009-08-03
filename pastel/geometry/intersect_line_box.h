// Description: The intersection between a line and a box

#ifndef PASTEL_INTERSECT_LINE_BOX_H
#define PASTEL_INTERSECT_LINE_BOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/box.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersection between a line and an box.
	/*!
	Post conditions:
	If returns true, hitList[0] <= hitList[1].
	*/

	template <int N, typename Real>
		bool intersect(
			const Line<N, Real>& line,
			const Box<N, Real>& box,
			Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect_line_box.hpp"

#endif
