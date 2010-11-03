// Description: Algorithms for rectangles
// Detail: intersect, etc.

#ifndef PASTEL_RECTANGLE_TOOLS_H
#define PASTEL_RECTANGLE_TOOLS_H

#include "pastel/sys/rectangle.h"
#include "pastel/sys/positionvisitor_concept.h"

namespace Pastel
{

	template <int N>
	bool intersect(
		const Rectangle<N>& aBox,
		const Rectangle<N>& bBox,
		Rectangle<N>& result);

	//! Visits each point contained in a rectangle.
	/*!
	region:
	Rectangle bounding the points to visit.
	
	positionVisitor:
	The functor to call on each position vector.
	See 'positionvisitor_concept.txt'.

	Returns:
	The number of visited points.
	
	Calls 'positionVisitor(position)' on each position
	vector.
	*/
	template <int N, typename PositionVisitor>
	void forEach(
		const Rectangle<N>& region,
		PositionVisitor positionVisitor);

}

#include "pastel/sys/rectangle_tools.hpp"

#endif
