// Description: Forward range concept
// Documentation: range_concepts.txt

#ifndef PASTEL_FORWARDRANGE_CONCEPT_H
#define PASTEL_FORWARDRANGE_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Range_Concept
	{

		class ForwardRange
		{
		public:
			// The type of the contained elements.
			typedef UserDefinedType value_type;
			// A reference to a contained element.
			typedef UserDefinedType& reference;
			// Whether this is a random-access range.
			static const bool RandomAccess = UserDefinedBoolean;
			// Whether this is a bidirectional range.
			static const bool Bidirectional = UserDefinedBoolean;

			//! Swaps two ranges.
			void swap(ForwardRange& that);

			//! Returns true if there are no elements in the range.
			bool empty() const;

			//! Removes the first element from the range.
			/*!
			Precondition:
			!empty()
			*/
			void pop_front();

			//! Returns the first element.
			/*!
			Precondition:
			!empty()
			*/
			reference front() const;

			//! Returns the number of elements in the range.
			integer size() const;
		};

	}

}

#endif
