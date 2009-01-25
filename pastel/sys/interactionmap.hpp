#ifndef PASTELSYS_INTERACTIONMAP_HPP
#define PASTELSYS_INTERACTIONMAP_HPP

#include "pastel/sys/interactionmap.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename First, typename Second>
	typename InteractionMap<First, Second>::ConstFirstIterator
		InteractionMap<First, Second>::firstBegin() const
	{
		return firsts_.begin();
	}

	template <typename First, typename Second>
	typename InteractionMap<First, Second>::ConstFirstIterator
		InteractionMap<First, Second>::firstEnd() const
	{
		return firsts_.end();
	}

	template <typename First, typename Second>
	typename InteractionMap<First, Second>::ConstSecondIterator
		InteractionMap<First, Second>::secondBegin() const
	{
		return seconds_.begin();
	}

	template <typename First, typename Second>
	typename InteractionMap<First, Second>::ConstSecondIterator
		InteractionMap<First, Second>::secondEnd() const
	{
		return seconds_.end();
	}

	template <typename First, typename Second>
	void InteractionMap<First, Second>::addInteraction(
		const First& first,
		const Second& second)
	{
		integer rollBackIndex = 0;

		SecondListRef secondList;
		FirstListRef firstList;

		bool createdFirstList = false;
		bool createdSecondList = false;

		SecondIterator secondIter(seconds_.find(second));
		FirstIterator firstIter(firsts_.find(first));

		try
		{
			if (secondIter == seconds_.end())
			{
				firstList.reset(new FirstList);
				secondIter = seconds_.insert(
					std::make_pair(second, firstList)).first;
				createdFirstList = true;
			}
			else
			{
				firstList = secondIter->second;
			}
			++rollBackIndex;

			if (firstIter == firsts_.end())
			{
				secondList.reset(new SecondList);
				firstIter = firsts_.insert(
					std::make_pair(first, secondList)).first;
				createdSecondList = true;
			}
			else
			{
				secondList = firstIter->second;
			}
			++rollBackIndex;

			secondList->insert(second);
			++rollBackIndex;

			firstList->insert(first);
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 4:
				firstList->erase(first);
				// Fall through
			case 3:
				secondList->erase(second);
				// Fall through
			case 2:
				if (createdSecondList)
				{
					firsts_.erase(firstIter);
				}
				// Fall through
			case 1:
				if (createdFirstList)
				{
					seconds_.erase(secondIter);
				}
				break;
			};
			throw;
		}
	}

	template <typename First, typename Second>
	void InteractionMap<First, Second>::removeInteraction(
		const First& first,
		const Second& second)
	{
		SecondIterator secondIter(seconds_.find(second));
		FirstIterator firstIter(firsts_.find(first));

		if (secondIter != seconds_.end())
		{
			ASSERT(firstIter != firsts_.end());

			secondIter->second->erase(first);
			firstIter->second->erase(second);
		}
	}

	template <typename First, typename Second>
	void InteractionMap<First, Second>::removeFirst(
		const First& first)
	{
		FirstIterator firstIter(firsts_.find(first));

		if (firstIter != firsts_.end())
		{
			firstIter->second->clear();
		}
	}

	template <typename First, typename Second>
	void InteractionMap<First, Second>::removeSecond(
		const Second& second)
	{
		SecondIterator secondIter(seconds_.find(second));

		if (secondIter != seconds_.end())
		{
			secondIter->second->clear();
		}
	}

}

#endif
