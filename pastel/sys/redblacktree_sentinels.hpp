#ifndef PASTELSYS_REDBLACKTREE_SENTINELS_HPP
#define PASTELSYS_REDBLACKTREE_SENTINELS_HPP

namespace Pastel
{
	
	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::splitSentinels()
	{
		if (hasSeparateSentinels())
		{
			// Nothing to do.
			return;
		}

		ENSURE(empty());

		bottom_ = std::make_shared<Sentinel_Node>();
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::mergeSentinels()
	{
		if (!hasSeparateSentinels())
		{
			// Nothing to do.
			return;
		}

		ENSURE(!sharesBottom());

		if (!empty())
		{
			bottomNode()->parent() = endNode()->parent();
			bottomNode()->right() = endNode()->right();

			// Here we replace the end-node with the
			// bottom-node, because otherwise we would have
			// to replace all references to the bottom-node
			// with the end-node in the elements.
			end_ = bottom_;
		}
		else
		{
			// Here we replace the bottom-node with the
			// end-node, because this way we preserve the 
			// end-node.
			bottom_ = end_;
			rootNode() = endNode();
		}
	}

}

#endif
