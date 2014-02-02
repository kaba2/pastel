// Description: Ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_RING_CONCEPT_H
#define PASTELSYS_RING_CONCEPT_H

#include "pastel/sys/additive_group_concept.h"

namespace Pastel
{

	namespace Ring_Concept
	{

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
