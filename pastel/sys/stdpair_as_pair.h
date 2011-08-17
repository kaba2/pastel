// Description: std::pair as a Pair
// Documentation: as_pair_concept.txt

#ifndef PASTEL_STDPAIR_AS_PAIR_H
#define PASTEL_STDPAIR_AS_PAIR_H

#include "pastel/sys/as_pair_concept.h"

namespace Pastel
{

	class StdPair_As_Pair
	{
	public:
		template <typename StdPair>
		class ConceptMap
		{
		public:
			typedef typename StdPair::first_type Left;
			typedef typename StdPair::second_type Right;
			typedef StdPair Type;

			Left& left(Type& that)
			{
				return that.first;
			}

			const Left& left(const Type& that)
			{
				return that.first;
			}

			Right& right(Type& that)
			{
				return that.second;
			}

			const Right& right(const Type& that)
			{
				return that.second;
			}			
		};
	};

}

#endif
