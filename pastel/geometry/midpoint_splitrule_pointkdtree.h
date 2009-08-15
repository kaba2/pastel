#ifndef PASTEL_MIDPOINT_SPLITRULE_POINTKDTREE_H
#define PASTEL_MIDPOINT_SPLITRULE_POINTKDTREE_H

namespace Pastel
{

	//! Refines a kd-tree using the midpoint rule.
	/*!
	Preconditions:
	maxDepth >= 0
	maxObjects >= 1

	The midpoint rule splits a node
	from the middle along the axis on which the node
	has greatest extent.

	This refinement is naive and should not
	be used for anything. However, it is provided for 
	checking and performance comparison purposes.
	*/

	class Midpoint_SplitRule_PointKdTree;

}

#include "pastel/geometry/midpoint_splitrule_pointkdtree.hpp"

#endif
