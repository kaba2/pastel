// Description: Temporal kd-tree invariants
// Documentation: tdtree.txt

#ifndef PASTELGEOMETRY_TDTREE_INVARIANTS_H
#define PASTELGEOMETRY_TDTREE_INVARIANTS_H

#include "pastel/geometry/tdtree/tdtree.h"

namespace Pastel
{

	//! Returns whether the invariants hold for the temporal kd-tree.
	/*!
	Time complexity: O(n log(n))
	
	This function is useful only for testing; a correct implementation
	always returns true.
	*/
	template <
		typename Settings,
		template <typename> class Customization>	
	bool testInvariants(const TdTree<Settings, Customization>& tree);

}

#include "pastel/geometry/tdtree/tdtree_invariants.hpp"

#endif
