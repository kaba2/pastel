#ifndef PASTELSYS_LIST_INVARIANTS_HPP
#define PASTELSYS_LIST_INVARIANTS_HPP

#include "pastel/sys/list/list_invariants.h"

#include <iterator>

namespace Pastel
{
	
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(
		const List<Settings, Customization>& list)
	{
		if ((list.size() == 0) != list.empty())
		{
			// The size() is zero if and only if the list
			// is empty.
			return false;
		}

		if (std::distance(list.cbegin(), list.cend()) != list.size())
		{
			// The size() must equal the number of elements
			// in the list.
			return false;
		}

		if (!list.cend().isEnd())
		{
			// The end-node must be locally identifiable 
			// as an end-node.
			return false;
		}

		if (!list.cbegin().isBegin())
		{
			// The begin-node must be locally identifiable
			// as a begin-node.
			return false;
		}

		if (list.empty())
		{
			return true;
		}

		auto begin = list.cbegin();
		auto end = list.cend();

		if (!begin.prev().isEnd())
		{
			// The previous of the first node must
			// be the end-node.
			return false;
		}
		
		if (end.next() != end)
		{
			// The next of the end-node must be 
			// the end-node.
			return false;
		}

		auto iter = begin;
		while (iter != end)
		{
			if (iter.empty() ||
				iter.isEnd() ||
				(iter.isBegin() != (iter == begin)) ||
				!iter.isNormal())
			{
				// An iterator must not be null or end, 
				// and must be normal. An iterator is
				// begin if and only if it is the begin
				// iterator.
				return false;
			}

			if (iter != begin &&
				iter.prev().next() != iter)
			{
				// The next of the previous must
				// be the element itself.
				return false;
			}

			if (iter.next().prev() != iter)
			{
				// The previous of the next must
				// be the element itself.
				return false;
			}

			++iter;
		}

		return true;
	}

}

#endif
