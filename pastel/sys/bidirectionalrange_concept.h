// Description: Bidirectional range concept
// Documentation: range_concepts.txt

#ifndef PASTEL_BIDIRECTIONALRANGE_CONCEPT_H
#define PASTEL_BIDIRECTIONALRANGE_CONCEPT_H

#include "pastel/sys/forwardrange_concept.h"

namespace Pastel
{

	namespace Range_Concept
	{

		class BidirectionalRange
			: public ForwardRange
		{
			//! Removes the last element from the range.
			/*!
			Preconditions:
			!empty()
			*/
			void pop_back();

			//! Returns the last element.
			/*!
			Preconditions:
			!empty()
			*/
			reference back() const;
		};

	}

}

#endif
