// Description: Range tree invariants
// Documentation: rangetree.txt

#ifndef PASTELGEOMETRY_RANGETREE_INVARIANTS_H
#define PASTELGEOMETRY_RANGETREE_INVARIANTS_H

#include "pastel/geometry/rangetree.h"

namespace Pastel
{

	//! Returns whether the invariants hold for the range tree.
	/*!
	Time complexity: O(n log(n))
	
	This function is useful only for testing; a correct implementation
	always returns true.
	*/
	template <
		typename Settings,
		template <typename> class Customization>	
	bool testInvariants(const RangeTree<Settings, Customization>& tree);

}

#include "pastel/geometry/rangetree_invariants.hpp"

#endif
