// Description: Types for the red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_FWD_H
#define PASTELSYS_REDBLACKFOREST_FWD_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblackforest_iterator.h"

#include <list>

namespace Pastel
{

	template <typename, template <typename> class>
	class RedBlackForest;

	template <typename Settings>
	class RedBlackForest_Fwd
	{
	public:
		using Fwd = Settings;

		using User_ElementSet = typename Settings::ElementSet;
		using User_SentinelData = typename User_ElementSet::Settings::SentinelData;

		class ElementSet_Settings;
		using ElementSet = RedBlackTree<ElementSet_Settings>;

		using SetSet = std::list<ElementSet>;
		using Set_Iterator = typename SetSet::iterator;
		using Set_ConstIterator = typename SetSet::const_iterator;

		class ElementSet_SentinelData
		: public Class<User_SentinelData>
		{
		private:
			template <typename, template <typename> class>
			friend class RedBlackForest;

			friend class ConstIterator;
			friend class Iterator;

			Set_Iterator set;
		};

		class ElementSet_Settings
		: public User_ElementSet::Settings
		{
		public:
			// Override the sentinel data, but preserve everything else.
			using SentinelData = ElementSet_SentinelData;
		};

		using Element_Iterator = typename ElementSet::Iterator;
		using Element_ConstIterator = typename ElementSet::ConstIterator;

		using ConstIterator = RedBlackForest_::Continuous_Iterator<Element_ConstIterator>;
		using Iterator = RedBlackForest_::Continuous_Iterator<Element_Iterator>;
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;
	};

}

#endif
