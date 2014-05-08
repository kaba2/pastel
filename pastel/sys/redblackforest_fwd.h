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

		using User_Tree = typename Settings::Tree;
		using User_SentinelData = typename User_Tree::Settings::SentinelData;

		class Tree_Settings;
		using Tree = RedBlackTree<Tree_Settings>;

		using TreeSet = std::list<Tree>;
		using Tree_Iterator = typename TreeSet::iterator;
		using Tree_ConstIterator = typename TreeSet::const_iterator;

		class Tree_SentinelData
		: public Class<User_SentinelData>
		{
		private:
			template <typename, template <typename> class>
			friend class RedBlackForest;

			friend class ConstIterator;
			friend class Iterator;

			Tree_Iterator tree;
		};

		class Tree_Settings
		: public User_Tree::Settings
		{
		public:
			// Override the sentinel data, but preserve everything else.
			using SentinelData = Tree_SentinelData;
		};

		using Element_Iterator = typename Tree::Iterator;
		using Element_ConstIterator = typename Tree::ConstIterator;

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
