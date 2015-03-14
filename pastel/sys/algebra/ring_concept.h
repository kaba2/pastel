// Description: Ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_RING_CONCEPT_H
#define PASTELSYS_RING_CONCEPT_H

#include "pastel/sys/algebra/additive_group_concept.h"

namespace Pastel
{

	//! A ring.
	/*!
	A ring is a triple (X, +, *) such that
		
	1) (X, +) is an additive group,
		
	2) * : X^2 --> X, called multiplication, such
	that
		
		(x + y) * z = x * z + y * z
		z * (x + y) = z * x + z * y
		
	for all x, y, z in X, and
		
	3) there exists an element 1 in X, called the 
	multiplicative identity, such that 
		
		1 * x = x = x * 1, for all x in X.
	*/
	struct Ring_Concept
	: Refines<Additive_Group_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Multiplies the element with 'that'.
				Concept::hasType<Type&>(t *= t),
				//! Adds 1 to the element, returns itself.
				Concept::hasType<Type&>(++t),
				//! Adds 1 to the element, returns the previous value.
				Concept::convertsTo<Type>(t++),
				//! Subtracts 1 from the element, returns itself.
				Concept::hasType<Type&>(--t),
				//! Subtracts 1 from the element, returns the previous value.
				Concept::convertsTo<Type>(t--),
				//! Returns left * right.
				Concept::convertsTo<Type>(t * t)
			)
		);
	};

}

#endif
