// Description: As_Pair concept

#ifndef PASTEL_AS_PAIR_CONCEPT_H
#define PASTEL_AS_PAIR_CONCEPT_H

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
				typedef UserDefinedType Left;
				typedef UserDefinedType Right;
				typedef Type_ Type;

				Left& left(Type& that);
				const Left& left(const Type& that);

				Right& right(Type& that);
				const Right& right(const Type& that);
			};
		};

	}

}

#endif
