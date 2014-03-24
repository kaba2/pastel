#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree()
		: root_(0)
		, endSentinel_(new EndSentinel)
		, childSentinel_(new ChildSentinel)
		, size_(0)
		, blackHeight_(0)
	{
		initialize();
	}

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that)
		: RedBlackTree()
	{
		try
		{
			Node* root = copyConstruct(endSentinel(), that, that.rootNode());
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
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(root_, that.root_);
		endSentinel_.swap(that.endSentinel_);
		childSentinel_.swap(that.childSentinel_);
		swap(size_, that.size_);
		swap(blackHeight_, that.blackHeight_);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		endSentinel_->parent() = endSentinel();
		endSentinel_->left() = endSentinel();
		endSentinel_->right() = endSentinel();
		endSentinel_->setRed(false);

		childSentinel_->parent() = childSentinel();
		childSentinel_->left() = childSentinel();
		childSentinel_->right() = childSentinel();
		childSentinel_->setRed(false);

		setMinimum(endSentinel());
		setMaximum(endSentinel());
		setRoot(endSentinel());

		size_ = 0;
		blackHeight_ = 0;
	}

}

#endif
