#ifndef PASTELSYS_REDBLACKTREE_SENTINELS_HPP
#define PASTELSYS_REDBLACKTREE_SENTINELS_HPP

namespace Pastel
{
	
	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::splitSentinels()
	{
		ENSURE(empty());

		if (hasSeparateSentinels())
		{
			// Nothing to do.
			return;
		}

		bottom_ = std::make_shared<Sentinel_Node>();
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::mergeSentinels()
	{
		ENSURE(!sharesBottom());

		if (!hasSeparateSentinels())
		{
			// Nothing to do.
			return;
		}

		if (!empty())
		{
			bottomNode()->parent() = endNode()->parent();
			bottomNode()->right() = endNode()->right();
		}
		else
		{
			bottomNode()->isolateSelf();
			rootNode() = bottomNode();
		}

		end_ = bottom_;
	}

}

#endif
