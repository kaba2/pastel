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

		end_ = allocateSentinel();
		rootNode() = endNode();
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
		}
		else
		{
			bottomNode()->isolateSelf();
			rootNode() = bottomNode();
		}

		deallocateSentinel(endNode());
		end_ = bottom_.get();
	}

}

#endif
