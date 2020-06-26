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
	xMin, xMax dreal numbers, y an integer.
	Which pixels should be set? Answer:
	[toPixelSpanPoint(xMin), toPixelSpanPoint(xMax)[
	on row y.
	*/
	integer toPixelSpanPoint(dreal t);

	template <integer N>
	Vector<integer, N> toPixelSpanPoint(
		const Vector<dreal, N>& that);

}

#include "pastel/sys/syscommon.hpp"

#endif
