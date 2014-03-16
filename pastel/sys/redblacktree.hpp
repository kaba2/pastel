#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree()
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
	{
		sentinel_ = new Node(0);
		initialize();
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that)
		: RedBlackTree()
	{
		try
		{
			root_ = copyConstruct(sentinel_, that, that.root_);
		}
		catch(...)
		{
			initialize();
			throw;
		}
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::~RedBlackTree()
	{
		clear();

		delete sentinel_;
		sentinel_ = 0;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(root_, that.root_);
		swap(sentinel_, that.sentinel_);
		swap(minimum_, that.minimum_);
		swap(size_, that.size_);
	}

	template <typename Settings, typename Customization>
	template <typename Key_ConstIterator_>
	void RedBlackTree<Settings, Customization>::insertMany(
		Key_ConstIterator_ begin, Key_ConstIterator_ end)
	{
		while(begin != end)
		{
			insert(*begin);
			++begin;
		}
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		sentinel_->parent() = sentinel_;
		sentinel_->left() = sentinel_;
		sentinel_->right() = sentinel_;

		setMinimum(sentinel_);
		setMaximum(sentinel_);
		
		root_ = sentinel_;
		size_ = 0;
	}

}

#endif
