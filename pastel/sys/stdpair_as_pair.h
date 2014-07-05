// Description: std::pair as a Pair
// Documentation: as_pair.txt

#ifndef PASTELSYS_STDPAIR_AS_PAIR_H
#define PASTELSYS_STDPAIR_AS_PAIR_H

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
			using Left = typename StdPair::first_type;
			using Right = typename StdPair::second_type;
			using Type = StdPair;

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
