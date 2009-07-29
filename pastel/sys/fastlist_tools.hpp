#ifndef PASTEL_FASTLIST_TOOLS_HPP
#define PASTEL_FASTLIST_TOOLS_HPP

#include "pastel/sys/fastlist_tools.h"

namespace Pastel
{

	template <typename Type, typename UniformAllocator, typename Predicate>
	std::pair<std::pair<typename FastList<Type, UniformAllocator>::iterator, integer>,
		std::pair<typename FastList<Type, UniformAllocator>::iterator, integer> >
		partition(
		FastList<Type, UniformAllocator>& list,
		const typename FastList<Type, UniformAllocator>::const_iterator& from,
		const typename FastList<Type, UniformAllocator>::const_iterator& to,
		const Predicate& predicate)
	{
		typedef FastList<Type, UniformAllocator> List;
		typedef typename List::iterator Iterator;
		typedef typename List::const_iterator ConstIterator;

		// The idea is to construct two lists,
		// those that fulfill the predicate and
		// those that don't.
		// The objects are not copied: instead
		// they are spliced between the lists.
		// In the end the individual lists
		// are spliced back to the original list.

		List trueList(list.get_allocator());
		List falseList(list.get_allocator());
		List trueBothList(list.get_allocator());
		List falseBothList(list.get_allocator());

		integer trueCount = 0;
		integer falseCount = 0;
		integer trueBothCount = 0;
		integer falseBothCount = 0;

		ConstIterator iter(from);
		while(iter != to)
		{
			ConstIterator next = iter;
			++next;

			TriState::Enum side = predicate(*iter);
			if (side == TriState::True)
			{
				trueList.splice(trueList.end(), list, iter);
				++trueCount;
			}
			else if (side == TriState::False)
			{
				falseList.splice(falseList.end(), list, iter);
				++falseCount;
			}
			else
			{
				if (trueBothCount < falseBothCount)
				{
					trueBothList.splice(trueBothList.end(), list, iter);
					++trueBothCount;
				}
				else
				{
					falseBothList.splice(falseBothList.end(), list, iter);
					++falseBothCount;
				}
			}

			iter = next;
		}

		if (trueCount < falseCount)
		{
			trueList.splice(trueList.end(), falseBothList);
			falseList.splice(falseList.end(), trueBothList);
			std::swap(trueBothCount, falseBothCount);
		}
		else
		{
			trueList.splice(trueList.end(), trueBothList);
			falseList.splice(falseList.end(), falseBothList);
		}

		// Splice back to the original list.

		ConstIterator falseStart = falseList.begin();
		if (falseList.empty())
		{
			falseStart = to;
		}

		ConstIterator trueStart = trueList.begin();
		if (trueList.empty())
		{
			trueStart = falseStart;
		}

		list.splice(to, trueList);
		list.splice(to, falseList);

		return std::make_pair(
			std::make_pair(list.cast(trueStart), trueCount + trueBothCount),
			std::make_pair(list.cast(falseStart), falseCount + falseBothCount));
	}

	template <typename Type, typename UniformAllocator, typename Predicate>
	std::pair<std::pair<typename FastList<Type, UniformAllocator>::iterator, integer>,
		std::pair<typename FastList<Type, UniformAllocator>::iterator, integer> >
		fuzzyPartition(
		FastList<Type, UniformAllocator>& list,
		const typename FastList<Type, UniformAllocator>::const_iterator& from,
		const typename FastList<Type, UniformAllocator>::const_iterator& to,
		const Predicate& predicate)
	{
		typedef FastList<Type, UniformAllocator> List;
		typedef typename List::iterator Iterator;
		typedef typename List::const_iterator ConstIterator;

		// The idea is to construct two lists,
		// those that fulfill the predicate and
		// those that don't. Because the predicate is
		// tertiary, there are also those that will get
		// included in both lists.
		// When the object goes to one list
		// only, it can be spliced if the lists
		// share an allocator.
		// When the object goes to both lists,
		// one extra copy needs to be done for the other
		// list, while for the other splicing can be used.
		// In the end, the invidual lists are again
		// spliced back to the original list.

		if (from == to)
		{
			// Nothing to do.

			return std::make_pair(
				std::make_pair(list.end(), 0),
				std::make_pair(list.end(), 0));
		}

		List trueList(list.get_allocator());
		List falseList(list.get_allocator());

		integer trueCount = 0;
		integer falseCount = 0;

		ConstIterator iter(from);
		while(iter != to)
		{
			ConstIterator next = iter;
			++next;

			const TriState::Enum side = predicate(*iter);
			if (side == TriState::True)
			{
				trueList.splice(trueList.end(), list, iter);
				++trueCount;
			}
			else if (side == TriState::False)
			{
				falseList.splice(falseList.end(), list, iter);
				++falseCount;
			}
			else if (side == TriState::Both)
			{
				trueList.push_back(*iter);
				++trueCount;
				falseList.splice(falseList.end(), list, iter);
				++falseCount;
			}

			iter = next;
		}

		// Splice back to the original list.

		ConstIterator falseStart = falseList.begin();
		if (falseList.empty())
		{
			falseStart = to;
		}

		ConstIterator trueStart = trueList.begin();
		if (trueList.empty())
		{
			trueStart = falseStart;
		}

		list.splice(to, trueList);
		list.splice(to, falseList);

		return std::make_pair(
			std::make_pair(list.cast(trueStart), trueCount),
			std::make_pair(list.cast(falseStart), falseCount));
	}

}

#endif
