// Description: Ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_RING_CONCEPT_H
#define PASTELSYS_RING_CONCEPT_H

#include "pastel/sys/algebra/group_concept.h"
#include "pastel/sys/algebra/monoid_concept.h"

namespace Pastel
{

	//! A ring.
	/*!
	A ring is a triple (X, +, 0, *, 1) such that
		
	1) (X, +, 0) is a commutative group,

	2) (X, *, 1) is a monoid,
		
	3) 
		(x + y) * z = x * z + y * z
		z * (x + y) = z * x + z * y
		
	for all x, y, z in X, and
	*/
	template <typename T>
	concept Ring_Concept_ = 
		Additive_Group_Concept<T> && 
		Multiplicative_Monoid_Concept<T> && 
		requires(T t) {
		//! Adds 1 to the element, returns itself.
		{++t} -> std::convertible_to<T>;
		//! Adds 1 to the element, returns the previous value.
		{t++} -> std::convertible_to<T>;
		//! Subtracts 1 from the element, returns itself.
		{--t} -> std::convertible_to<T>;
		//! Subtracts 1 from the element, returns the previous value.
		{t--} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Ring_Concept = 
		Ring_Concept_<RemoveCvRef<T>>;

}

#endif
