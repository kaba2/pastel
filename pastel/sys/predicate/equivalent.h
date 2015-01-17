// Description: Equivalence
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_EQUIVALENT_H
#define PASTELSYS_EQUIVALENT_H

#include "pastel/sys/algebra/less_concept.h"

namespace Pastel
{

	//! Returns whether elements are equivalent under a strict weak order.
	/*!
	left, right:
	The elements to compare.

	less:
	A strict weak order.
	*/
	template <typename Type, typename Less>
	bool equivalent(const Type& left, const Type& right, 
		const Less& less)
	{
		return !less(left, right) && !less(right, left);
	}

}

#endif
