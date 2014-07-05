#ifndef PASTELSYS_LIST_PARTITION_HPP
#define PASTELSYS_LIST_PARTITION_HPP

#include "pastel/sys/list_partition.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization,
		typename Predicate>
	std::pair<std::pair<typename List<Settings, Customization>::iterator, integer>,
		std::pair<typename List<Settings, Customization>::iterator, integer> >
		partition(
		List<Settings, Customization>& list,
		const typename List<Settings, Customization>::ConstRange& range,
		const Predicate& predicate)
	{
		using List = List<Settings, Customization>;
		using Iterator = typename List::iterator;
		using ConstIterator = typename List::const_iterator;

		// The idea is to construct two lists,
		// those that fulfill the predicate and
		// those that don't.
		// The objects are not copied: instead
		// they are spliced between the lists.
		// In the end the individual lists
		// are spliced back to the original list.

		List trueList;
		List falseList;
		List trueBothList;
		List falseBothList;

		integer trueCount = 0;
		integer falseCount = 0;
		integer trueBothCount = 0;
		integer falseBothCount = 0;

		ConstIterator iter = std::begin(range);
		ConstIterator to = std::end(range);
		while(iter != to)
		{
			ConstIterator next = iter;
			++next;

			TriState side = predicate(*iter);
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

	template <
		typename Settings,
		template <typename> class Customization,
		typename Predicate>
	std::pair<std::pair<typename List<Settings, Customization>::iterator, integer>,
		std::pair<typename List<Settings, Customization>::iterator, integer> >
		fuzzyPartition(
		List<Settings, Customization>& list,
		const typename List<Settings, Customization>::ConstRange& range,
		const Predicate& predicate)
	{
		using List = List<Settings, Customization>;
		using Iterator = typename List::iterator;
		using ConstIterator = typename List::const_iterator;

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

		if (range.empty())
		{
			// Nothing to do.

			return std::make_pair(
				std::make_pair(list.end(), 0),
				std::make_pair(list.end(), 0));
		}

		List trueList;
		List falseList;

		integer trueCount = 0;
		integer falseCount = 0;

		ConstIterator iter = std::begin(range);
		ConstIterator to = std::end(range);
		while(iter != to)
		{
			ConstIterator next = iter;
			++next;

			const TriState side = predicate(*iter);
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
