#ifndef PASTEL_DISJOINTGROUPS_HPP
#define PASTEL_DISJOINTGROUPS_HPP

#include "pastel/sys/disjointgroups.h"

namespace Pastel
{

	// Elements

	template <typename Type>
	DisjointGroups<Type>::DisjointGroups()
		: groupSet_()
		, elementList_()
	{
	}

	template <typename Type>
	DisjointGroups<Type>::DisjointGroups(const DisjointGroups& that)
		: groupSet_(that.groupSet_)
		, elementList_(that.elementList_)
	{
		ENSURE(false);
	}

	template <typename Type>
	DisjointGroups<Type>& DisjointGroups<Type>::operator=(const DisjointGroups& that)
	{
		DisjointGroups copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type>
	void DisjointGroups<Type>::swap(DisjointGroups& that)
	{
		elementList_.swap(that.elementList_);
		groupSet_.swap(that.groupSet_);
	}

	template <typename Type>
	void DisjointGroups<Type>::clear()
	{
		elementList_.clear();
		groupSet_.clear();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ElementIterator
		DisjointGroups<Type>::elementBegin()
	{
		return elementList_.begin();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ConstElementIterator
		DisjointGroups<Type>::elementBegin() const
	{
		return elementList_.begin();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ElementIterator
		DisjointGroups<Type>::elementEnd()
	{
		return elementList_.end();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ConstElementIterator
		DisjointGroups<Type>::elementEnd() const
	{
		return elementList_.end();
	}

	template <typename Type>
	void DisjointGroups<Type>::erase(const ElementIterator& element)
	{
		GroupIterator group = element.group_;

		if (group.first_ == element)
		{
			if (group.last_ == element)
			{
				group.first_ = elementList_.end();
				group.last_ = elementList_.end();
			}
			else
			{
				++group.first_;
			}
		}
		else
		{
			if (group.last_ == element)
			{
				group.last_ = elementList_.end();
			}
			// Else the element is
			// at the middle of the list, do nothing.
		}

		elementList_.erase(element);
		--group.size_;
	}

	template <typename Type>
	integer DisjointGroups<Type>::elements() const
	{
		return elementList_.size();
	}

	// Groups

	template <typename Type>
	typename DisjointGroups<Type>::GroupIterator
		DisjointGroups<Type>::insertGroup()
	{
		groupSet_.push_back(
			Group(elementList_.end(), elementList_.end(), 0));

		GroupIterator result = groupSet_.end();
		--result;

		return result;
	}

	template <typename Type>
	typename DisjointGroups<Type>::GroupIterator
		DisjointGroups<Type>::groupBegin()
	{
		return groupSet_.begin();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ConstGroupIterator
		DisjointGroups<Type>::groupBegin() const
	{
		return groupSet_.begin();
	}

	template <typename Type>
	typename DisjointGroups<Type>::GroupIterator
		DisjointGroups<Type>::groupEnd()
	{
		return groupSet_.end();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ConstGroupIterator
		DisjointGroups<Type>::groupEnd() const
	{
		return groupSet_.end();
	}

	template <typename Type>
	void DisjointGroups<Type>::erase(const GroupIterator& group)
	{
		elementList_.erase(group->begin(), group->end());
		groupSet_.erase(group);
	}

	template <typename Type>
	integer DisjointGroups<Type>::groups() const
	{
		return groupSet_.size();
	}

	template <typename Type>
	typename DisjointGroups<Type>::ElementIterator
		DisjointGroups<Type>::insert(
		const GroupIterator& group,
		const Type& data)
	{
		PENSURE(group != groupSet_.end());

		ElementIterator element;

		if (group->size() == 0)
		{
			elementList_.push_back(Element(group, data));
			element = elementList_.end();
			--element;
			group->first_ = element;
			group->last_ = element;
		}
		else
		{
			ElementIterator groupEnd = group->last_;
			++groupEnd;
			element = elementList_.insert(groupEnd, Element(group, data));
			group->last_ = element;
		}

		++group->size_;

		return element;
	}

	template <typename Type>
	typename DisjointGroups<Type>::GroupIterator
		DisjointGroups<Type>::merge(
		const GroupIterator& aGroup,
		const GroupIterator& bGroup)
	{
		if (aGroup == bGroup)
		{
			return aGroup;
		}

		if (aGroup->empty())
		{
			groupSet_.erase(aGroup);
			return bGroup;
		}

		if (bGroup->empty())
		{
			groupSet_.erase(bGroup);
			return aGroup;
		}

		GroupIterator biggerGroup = aGroup;
		GroupIterator smallerGroup = bGroup;

		if (biggerGroup->size() < smallerGroup->size())
		{
			biggerGroup.swap(smallerGroup);
		}

		ElementIterator elementIter = smallerGroup->begin();
		ElementIterator elementEnd = smallerGroup->end();

		while(elementIter != elementEnd)
		{
			elementIter->group_ = biggerGroup;
			++elementIter;
		}

		elementList_.splice(biggerGroup->end(), elementList_,
			smallerGroup->begin(), smallerGroup->end());

		biggerGroup->last_ = smallerGroup->last_;
		biggerGroup->size_ += smallerGroup->size_;

		groupSet_.erase(smallerGroup);

		return biggerGroup;
	}

}

#endif
