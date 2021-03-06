// Description: As_Pair concept
// Documentation: as_pair.txt

#ifndef PASTELSYS_AS_PAIR_CONCEPT_H
#define PASTELSYS_AS_PAIR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace As_Pair_Concept
	{

		class As_Pair
		{
		public:
			template <typename Type_>
			class ConceptMap
			{
			public:
				//! The type of the first element.
				using Left = UserDefinedType;

				//! The type of the second element.
				using Right = UserDefinedType;

				//! The type of the concrete pair.
				using Type = Type_;

				//! Returns the first element of a pair.
				Left& left(Type& that);

				//! Returns the first element of a pair.
				const Left& left(const Type& that);

				//! Returns the second element of a pair.
				Right& right(Type& that);

				//! Returns the second element of a pair.
				const Right& right(const Type& that);
			};
		};

	}

}

#endif
