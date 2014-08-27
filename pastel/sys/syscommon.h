// Description: Miscellaneous tools
// Documentation: undocumented.txt

#ifndef PASTELSYS_SYSCOMMON_H
#define PASTELSYS_SYSCOMMON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	/*!
	Example:
	Given a 2d horizontal line segment [xMin, xMax[ on row y,
	xMin, xMax real numbers, y an integer.
	Which pixels should be set? Answer:
	[toPixelSpanPoint(xMin), toPixelSpanPoint(xMax)[
	on row y.
	*/
	integer toPixelSpanPoint(real t);

	template <int N>
	Vector<integer, N> toPixelSpanPoint(
		const Vector<real, N>& that);

}

#include "pastel/sys/syscommon.hpp"

#endif
