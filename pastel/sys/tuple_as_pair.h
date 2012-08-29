// Description: Tuple as a Pair
// Documentation: as_pair.txt

#ifndef PASTEL_TUPLE_AS_PAIR_H
#define PASTEL_TUPLE_AS_PAIR_H

#include "pastel/sys/as_pair_concept.h"

namespace Pastel
{

	class Tuple_As_Pair
	{
	public:
		template <typename Tuple>
		class ConceptMap
		{
		public:
			typedef typename Tuple::value_type Left;
			typedef typename Tuple::value_type Right;
			typedef Tuple Type;

			Left& left(Type& that) const
			{
				return that[0];
			}

			const Left& left(const Type& that) const
			{
				return that[0];
			}

			Right& right(Type& that) const
			{
				return that[1];
			}

			const Right& right(const Type& that) const
			{
				return that[1];
			}			
		};
	};

}

#endif
