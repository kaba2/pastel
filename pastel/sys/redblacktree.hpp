#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree()
		: minimum_(0)
		, sentinel_(0)
		, size_(0)
	{
		sentinel_ = new Node_Base(0);
		initialize();
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that)
		: RedBlackTree()
	{
		try
		{
			Node* root = copyConstruct((Node*)sentinel_, that, that.rootNode());
			setRoot(root);
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
		swap(minimum_, that.minimum_);
		swap(sentinel_, that.sentinel_);
		swap(size_, that.size_);
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		sentinel_->parent() = (Node*)sentinel_;
		sentinel_->left() = (Node*)sentinel_;
		sentinel_->right() = (Node*)sentinel_;

		setMinimum((Node*)sentinel_);
		setMaximum((Node*)sentinel_);
		setRoot((Node*)sentinel_);

		size_ = 0;
	}

}

#endif
