// Description: Types for the red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_FWD_H
#define PASTELSYS_REDBLACKFOREST_FWD_H

#include "pastel/sys/redblacktree.h"

#include <list>

namespace Pastel
{

	template <typename, template <typename> class>
	class RedBlackForest;

	namespace RedBlackForest_
	{

		template <typename>
		class Set;

	}

	template <typename Settings>
	class RedBlackForest_Fwd
	{
	public:
		using Fwd = Settings;
		
		PASTEL_FWD(Key);
		PASTEL_FWD(Data);
		PASTEL_FWD(Less);

		using Data_Class = Class<Data>;

		using ElementSet = RedBlack_Map<Key, Data, Less>;
		using Element_Iterator = typename ElementSet::Iterator;
		using Element_ConstIterator = typename ElementSet::ConstIterator;

		using Iterator = Element_Iterator;
		using ConstIterator = Element_ConstIterator;

		using Set = RedBlackForest_::Set<Settings>;

		using SetSet = std::list<Set>;
		using Set_Iterator = typename SetSet::iterator;
		using Set_ConstIterator = typename SetSet::const_iterator;
	};

}

#endif
