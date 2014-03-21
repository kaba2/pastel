#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree()
		: minimum_(0)
		, sentinel_(0)
		, size_(0)
		, blackHeight_(0)
	{
		sentinel_ = new Sentinel();
		initialize();
	}

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that)
		: RedBlackTree()
	{
		try
		{
			Node* root = copyConstruct((Node*)sentinel_, that, that.rootNode());
			setRoot(root);
			size_ = that.size_;
			blackHeight_ = that.blackHeight_;
		}
		catch(...)
		{
			initialize();
			throw;
		}
	}

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::~RedBlackTree()
	{
		clear();

		delete sentinel_;
		sentinel_ = 0;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(minimum_, that.minimum_);
		swap(sentinel_, that.sentinel_);
		swap(size_, that.size_);
		swap(blackHeight_, that.blackHeight_);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		sentinel_->parent() = (Node*)sentinel_;
		sentinel_->left() = (Node*)sentinel_;
		sentinel_->right() = (Node*)sentinel_;
		sentinel_->setRed(false);

		setMinimum((Node*)sentinel_);
		setMaximum((Node*)sentinel_);
		setRoot((Node*)sentinel_);

		size_ = 0;
		blackHeight_ = 0;
	}

}

#endif
