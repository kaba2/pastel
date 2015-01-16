// Description: Ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_RING_CONCEPT_H
#define PASTELSYS_RING_CONCEPT_H

#include "pastel/sys/additive_group_concept.h"

namespace Pastel
{

	namespace Ring_Concept
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
		class Ring
		: public Additive_Group_Concept::Additive_Group
		{
		public:
			//! Multiplies the element with 'that'.
			Ring& operator*=(const Ring& that);

			//! Adds 1 to the element.
			Ring& operator++();

			//! Adds 1 to the element, returns the previous value.
			Ring operator++(int);

			//! Subtracts 1 from the element.
			Ring& operator--();

			//! Subtracts 1 from the element, returns the previous value.
			Ring operator--(int);
		};

		//! Returns left * right.
		Ring operator*(
			const Ring& left,
			const Ring& right);

	}

}

#endif
