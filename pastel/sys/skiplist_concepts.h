// Description: Skip list concepts
// Documentation: skiplist.txt

#ifndef PASTELSYS_SKIPLIST_CONCEPTS_H
#define PASTELSYS_SKIPLIST_CONCEPTS_H

#include "pastel/sys/predicate_concept.h"

#include "pastel/sys/skiplist.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace SkipList_Concepts
	{

		class Settings
		{
		public:
			// The type of the key by which elements are ordered.
			using Key = UserDefinedType;

			//! The predicate used to compare keys.
			using Compare = Predicate_Concept::Predicate;

			//! The type of the data associated with a given key. 
			/*!
			Setting this to void avoids allocating any memory 
			for the associated data. In addition, while normally
			skip list iterators dereference to the value, 
			after setting Value to void, iterators dereference 
			to the key.
			*/
			using Value = UserDefinedType;

			//! Whether to allow multiple equal keys.
			enum {MultipleKeys = UserDefinedInteger};
		};

	}

}

#endif
