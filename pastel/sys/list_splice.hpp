#ifndef PASTELSYS_LIST_SPLICE_HPP
#define PASTELSYS_LIST_SPLICE_HPP

#include "pastel/sys/list.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	List<Settings, Customization>& 
		List<Settings, Customization>::splice(
		const ConstIterator& to,
		List& fromSet,
		const ConstIterator& begin,
		const ConstIterator& end)
	{
		integer count = 0;
		if (this != &fromSet)
		{
			count = std::distance(begin, end);
		}
		
		return splice(to, fromSet, begin, end, count);
	}

	template <
		typename Settings,
		template <typename> class Customization>
	List<Settings, Customization>& 
		List<Settings, Customization>::splice(
		const ConstIterator& to,
		List& fromSet,
		const ConstIterator& begin,
		const ConstIterator& end,
		integer count)
	{
		ASSERT_OP(count, >=, 0);

		Node* toNode = (Node*)to.base();
		Node* fromBegin = (Node*)begin.base();
		Node* fromEnd = (Node*)end.base();

		ASSERT(toNode);
		ASSERT(fromBegin);
		ASSERT(fromEnd);

		if (fromBegin == fromEnd ||
			toNode == fromBegin)
		{
			// Nothing to do.
			return *this;
		}

		// Remove range [from, toNode[ from fromSet.

		Node* fromBeginPrevious = fromBegin->prev();
		ASSERT(fromBeginPrevious);

		Node* fromEndPrevious = fromEnd->prev();
		ASSERT(fromEndPrevious);

		linkNodes(fromBeginPrevious, fromEnd);

		// Insert the range intoNode this

		Node* thisPrevious = toNode->prev();
		ASSERT(thisPrevious);

		linkNodes(thisPrevious, fromBegin);
		linkNodes(fromEndPrevious, toNode);

		// Update sizes
		size_ += count;
		fromSet.size_ -= count;

		return *this;
	}

}

#endif
