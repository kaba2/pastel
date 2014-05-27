// Description: Types for the red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_FWD_H
#define PASTELSYS_REDBLACKFOREST_FWD_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblackforest_iterator.h"
#include "pastel/sys/list.h"

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
		using User_Tree_Settings = Settings_For<User_Tree>;
		using User_Tree_Fwd = RedBlackTree_Fwd<User_Tree_Settings>;
		using User_SentinelData = typename User_Tree_Settings::SentinelData;

		class Tree_Settings;
		using Tree = Replace_Settings<User_Tree, Tree_Settings>;
		using Tree_Fwd = RedBlackTree_Fwd<Tree_Settings>;

		using TreeSet = List_Set<Tree, Tree>;
		using TreeSet_Fwd = List_Set_Fwd<Tree, Tree>;
		using Tree_Iterator = typename TreeSet_Fwd::Iterator;
		using Tree_ConstIterator = typename TreeSet_Fwd::ConstIterator;
		using Tree_Range = boost::iterator_range<Tree_Iterator>;
		using Tree_ConstRange = boost::iterator_range<Tree_ConstIterator>;

		class Tree_SentinelData
		: public As_Class<User_SentinelData>
		{
		public:
			Tree_SentinelData()
			: tree_()
			, isForestEnd_(false)
			{
			}

			~Tree_SentinelData() = default;

			Tree_Iterator tree()
			{
				return tree_;
			}

			Tree_ConstIterator tree() const
			{
				return tree_;
			}

			bool isForestEnd() const
			{
				return isForestEnd_;
			}

		private:
			template <typename, template <typename> class>
			friend class RedBlackForest;

			friend class ConstIterator;
			friend class Iterator;

			// Prohibit all ways of changing the 
			// data below from outside.
			Tree_SentinelData(const Tree_SentinelData&) = delete;
			Tree_SentinelData(Tree_SentinelData&&) = delete;
			Tree_SentinelData& operator=(Tree_SentinelData) = delete;

			//! An iterator, in the list of trees, corresponding to the tree.
			Tree_Iterator tree_;

			//! Whether this is the end-node of the sentinel tree.
			bool isForestEnd_;
		};

		class Tree_Settings
		: public User_Tree_Settings
		{
		public:
			// Override the sentinel data, but preserve everything else.
			using SentinelData = Tree_SentinelData;
		};

		using Element_Iterator = typename Tree_Fwd::Iterator;
		using Element_ConstIterator = typename Tree_Fwd::ConstIterator;

		using ConstIterator = RedBlackForest_::Iterator<Element_ConstIterator>;
		using Iterator = RedBlackForest_::Iterator<Element_Iterator>;
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;
	};

}

#endif
