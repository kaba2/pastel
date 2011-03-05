// Description: Random access range concept
// Documentation: range_concepts.txt

#ifndef PASTEL_RANDOMACCESSRANGE_CONCEPT_H
#define PASTEL_RANDOMACCESSRANGE_CONCEPT_H

#include "pastel/sys/bidirectionalrange_concept.h"

namespace Pastel
{

	namespace Range_Concept
	{

		class RandomAccessRange
			: public BidirectionalRange
		{
			//! Returns the index:th element.
			/*!
			Preconditions:
			index >= 0
			index < size()
			*/
			reference operator[](integer index) const;
		};

	}

}

#endif
