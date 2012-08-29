// Description: KeyValue as a Pair
// Documentation: as_pair.txt

#ifndef PASTEL_KEYVALUE_AS_PAIR_H
#define PASTEL_KEYVALUE_AS_PAIR_H

#include "pastel/sys/as_pair_concept.h"

namespace Pastel
{

	class KeyValue_As_Pair
	{
	public:
		template <typename KeyValue>
		class ConceptMap
		{
		public:
			typedef typename KeyValue::Key Left;
			typedef typename KeyValue::Value Right;
			typedef KeyValue Type;

			Left& left(Type& that)
			{
				return that.key();
			}

			const Left& left(const Type& that)
			{
				return that.key();
			}

			Right& right(Type& that)
			{
				return that.value();
			}

			const Right& right(const Type& that)
			{
				return that.value();
			}			
		};
	};

}

#endif
