#ifndef PASTELSYS_SKIPLIST_INVARIANTS_HPP
#define PASTELSYS_SKIPLIST_INVARIANTS_HPP

#include "pastel/sys/skiplist.h"

namespace Pastel
{

	template <typename SkipList_Settings>
	bool testInvariants(const SkipList<SkipList_Settings>& that)
	{
		if (that.empty())
		{
			if (that.cbegin() != that.cend())
			{
				return false;
			}

			if (that.size() > 0)
			{
				// The skip-list is empty, but still its size
				// is reportedly positive.
				return false;
			}

			if (that.uniqueKeys() > 0)
			{
				// The skip-list is empty, but still it 
				// reportedly contains unique keys.
				return false;
			}

			return true;
		}

		if (that.maxHeight() > 0 && that.height() > that.maxHeight() + 1)
		{
			// The skip list is higher than its allowed maximum height.
			return false;
		}

		using List = SkipList<SkipList_Settings>;
		using ConstIterator = typename List::ConstIterator;
		using Less = typename SkipList_Settings::Less;

		integer size = 1;
		integer uniqueKeys = 1;
		for (integer i = 0;i <= that.height();++i)
		{
			ConstIterator iter = that.cbegin(i);
			ConstIterator end = that.cend();

			// Check that the nodes are bidirectionally linked.
			while (iter != end)
			{
				if (iter.prev(i).next(i) != iter ||
					iter.next(i).prev(i) != iter)
				{
					// The nodes are not bidirectionally linked.
					return false;
				}

				iter = iter.next(i);
			}

			iter = that.cbegin(i);

			iter = iter.next(i);
			integer sameLevels = 1;
			while(iter != end)
			{
				ConstIterator prev = iter.prev(i);
				if (Less()(iter.key(), prev.key()))
				{
					// The elements are not in non-decreasing 
					// order of keys.
					return false;
				}

				if (i == 1 && !Less()(prev.key(), iter.key()))
				{
					// The level-1 does not contain unique keys.
					return false;
				}

				if (iter.height() == prev.height())
				{
					++sameLevels;
					if (sameLevels > 2 && iter.height() != that.maxHeight() + 1)
					{
						// The deterministic 1-2-skip-list must
						// have at most 2 subsequent elements
						// at the same level, when following
						// links on a given level. The exception to this
						// is the top height, in case the skip list
						// has a maximum height.
						return false;
					}
				}
				else
				{
					sameLevels = 1;
				}

				iter = iter.next(i);

				if (i == 0)
				{
					++size;
				}
				if (i == 1)
				{
					++uniqueKeys;
				}
			}
		}

		if (that.size() != size)
		{
			// The number of stored elements does not match
			// the reported size.
			return false;
		}

		if (that.uniqueKeys() != uniqueKeys)
		{
			// The number of unique elements does not match
			// the reported number.
			return false;
		}

		// No invariants were found to be broken.
		return true;
	}

}

#endif
