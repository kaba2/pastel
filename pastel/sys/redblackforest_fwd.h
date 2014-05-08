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
		using Tree_Range = boost::iterator_range<Tree_Iterator>;
		using Tree_ConstRange = boost::iterator_range<Tree_ConstIterator>;

		class Tree_SentinelData
		: public Class<User_SentinelData>
		{
		public:
			Tree_SentinelData() = default;
			~Tree_SentinelData() = default;

			Tree_Iterator tree()
			{
				return tree_;
			}

			Tree_ConstIterator tree() const
			{
				return tree_;
			}

		private:
			template <typename, template <typename> class>
			friend class RedBlackForest;

			friend class ConstIterator;
			friend class Iterator;

			// Prohibit all ways of changing the 'tree' field
			// from outside.
			Tree_SentinelData(const Tree_SentinelData&) = delete;
			Tree_SentinelData(Tree_SentinelData&&) = delete;
			Tree_SentinelData& operator=(Tree_SentinelData) = delete;

			//! An iterator, in the list of trees, corresponding to the tree.
			Tree_Iterator tree_;
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
