#ifndef PASTELSYS_REDBLACKTREE_SWAP_HPP
#define PASTELSYS_REDBLACKTREE_SWAP_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(root_, that.root_);
		swap(end_, that.end_);
		bottom_.swap(that.bottom_);
		swap(blackHeight_, that.blackHeight_);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swapElements(
		RedBlackTree& that)
	{
		ENSURE(sharesBottom(that));

		if (empty() && that.empty())
		{
			return;
		}

		if (empty() != that.empty())
		{
			RedBlackTree& emptyTree = empty() ?
				*this : that;
			RedBlackTree& fullTree = empty() ?
				that : *this;

			emptyTree.minNode() = fullTree.minNode();
			emptyTree.maxNode() = fullTree.maxNode();
			emptyTree.rootNode() = fullTree.rootNode();

			emptyTree.minNode()->left() = emptyTree.endNode();
			emptyTree.maxNode()->right() = emptyTree.endNode();
			emptyTree.rootNode()->parent() = emptyTree.endNode();
			
			std::swap(end_, that.end_);
			swap(that);

			fullTree.forget();

			return;
		}

		std::swap(
			rootNode()->parent(),
			that.rootNode()->parent());
		std::swap(
			minNode()->left(),
			that.minNode()->left());
		std::swap(
			maxNode()->right(),
			that.maxNode()->right());
		std::swap(
			endNode()->parent(),
			that.endNode()->parent());
		std::swap(
			endNode()->right(),
			that.endNode()->right());
		std::swap(end_, that.end_);
		swap(that);
	}

}

#endif
