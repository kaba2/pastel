// Description: Refinable partition

#ifndef PASTEL_REFINABLE_PARTITION_H
#define PASTEL_REFINABLE_PARTITION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/refinable_partition_fwd.h"
#include "pastel/sys/refinable_partition_element.h"
#include "pastel/sys/refinable_partition_block.h"

namespace Pastel
{

	//! Refinable partition
	/*!
	Preconditions:
	Type is move/copy-constructible.

	Note:
	Specifying Type as EmptyClass avoids allocating
	any memory for the data.
	*/
	template <typename Type = EmptyClass>
	class RefinablePartition
		: public RefinablePartition_Fwd<Type>
	{
	public:
		//! Constructs an empty partition.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		RefinablePartition()
			: elementSet_()
			, setSet_()
			, memberSet_()
			, splitSet_()
		{
		}

		//! Constructs a partition with the given elements.
		/*!
		Time complexity:
		O(std::distance(begin, end)) * copy/move-construct(Type)

		Exception safety:
		strong

		Note: 
		If begin == end, there will be no sets in the 
		partition. Otherwise, there will be exactly one set 
		in the partition, containing all the elements.
		*/
		template <typename Type_ConstIterator>
		RefinablePartition(
			const Type_ConstIterator& begin,
			const Type_ConstIterator& end)
			: elementSet_()
			, setSet_()
			, memberSet_()
			, splitSet_()
		{
			insert(begin, end);
		}

		//! Move-constructs from another partition.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		RefinablePartition(
			RefinablePartition&& that)
			: elementSet_()
			, setSet_()
			, memberSet_()
			, splitSet_()
		{
			swap(that);
		}

		//! Assigns from another partition.
		/*!
		Time complexity:
		O(that.elements()), if that was copy-constructed,
		constant, if that was move-constructed.

		Exception safety:
		strong
		*/
		RefinablePartition& operator=(
			RefinablePartition that)
		{
			swap(that);
			return *this;
		}

		//! Removes all elements and sets.
		/*!
		Time complexity:
		O(elements())

		Exception safety:
		nothrow
		*/
		void clear()
		{
			elementSet_.clear();
			setSet_.clear();
			memberSet_.clear();
			splitSet_.clear();
		}

		//! Swaps two partitions.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(RefinablePartition& that)
		{
			elementSet_.swap(that.elementSet_);
			setSet_.swap(that.setSet_);
			memberSet_.swap(that.memberSet_);
			splitSet_.swap(that.splitSet_);
		}

		// Sets

		//! Inserts an empty set into the partition.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		Set_ConstIterator insert()
		{
			return setSet_.emplace(
				setSet_.cend(), 
				Set(
				memberSet_.end(),
				memberSet_.end(),
				splitSet_.end(),
				0, false));
		}

		//! Removes a set from the partition.
		/*!
		returns:
		std::next(set)

		Time complexity:
		O(set->elements())

		Exception safety:
		nothrow
		*/
		Set_ConstIterator erase(
			const Set_ConstIterator& set)
		{
			// Remove all the elements and members
			// in the set.
			auto member = set->begin();
			auto memberEnd = set->end();
			while(member != memberEnd)
			{
				elementSet_.erase(*member);
				member = memberSet_.erase(member);
			}

			// Remove the set.
			return setSet_.erase(set);
		}

		// Elements

		//! Inserts elements into a new set.
		/*!
		This is a convenience function which calls
		insert(begin, end, setEnd()).
		*/
		template <typename Type_ConstIterator>
		Set_ConstIterator insert(
			const Type_ConstIterator& begin,
			const Type_ConstIterator& end)
		{
			return insert(begin, end, setEnd());
		}

		//! Inserts elements into a set.
		/*!
		returns:
		An iterator to the set into which the elements
		were inserted.

		Time complexity:
		O(std::distance(begin, end)) * copy/move-construct(Type)
		
		Exception safety:
		strong
		
		Note:
		If begin == end, then no set is created and 
		setEnd() is returned. If begin != end and 
		set == setEnd(), a new set is created for 
		the elements. If begin != end and set != setEnd(), 
		then the elements are inserted into the 
		given existing set.
		*/
		template <typename Type_ConstIterator>
		Set_ConstIterator insert(
			const Type_ConstIterator& begin,
			const Type_ConstIterator& end,
			Set_ConstIterator set)
		{
			if (begin == end)
			{
				// Nothing to do.
				return setSet_.cend();
			}

			if (set == setSet_.cend())
			{
				// Create an empty set into the partition.
				set = insert();
			}

			// We need to keep track of the number
			// created objects so that we can roll
			// back their creation on an exception.
			integer elementsCreated = 0;
			integer membersCreated = 0;

			// Create the elements and the initial partition.
			integer n = 0;
			for (auto iter = begin; iter != end; ++iter)
			{
				try
				{
					Element_Iterator element =
						elementSet_.emplace(
						elementSet_.cend(),
						Element(cast(set), 
						memberSet_.end(),
						*iter));
					++elementsCreated;

					Member_Iterator member =
						memberSet_.emplace(
						memberSet_.cend(),
						element);
					element->member_ = member;
					++membersCreated;
				}
				catch(...)
				{
					// Roll back element creation.
					for (integer i = 0;i < elementsCreated;++i)
					{
						elementSet_.pop_back();
					}

					// Roll back member creation.
					for (integer i = 0;i < membersCreated;++i)
					{
						memberSet_.pop_back();
					}

					// Roll back set creation.
					erase(set);

					throw;
				}

				// Also count the number of elements to n.
				++n;
			}

			// Set the set elements properly.
			cast(set)->set(
				memberSet_.begin(),
				memberSet_.end(),
				n);

			return set;
		}

		//! Removes an element from the partition.
		/*!
		returns:
		std::next(element)

		Note:
		If the containing set of the element becomes empty,
		the set is not removed.
		*/
		Element_ConstIterator erase(
			const Element_ConstIterator& element)
		{
			// Remove the element from its set.
			cast(element)->set_->erase(
				cast(element),
				memberSet_.end());

			// Remove the element.
			return elementSet_.erase(element);
		}

		//! Marks an element of a set.
		/*!
		This is a convenience function that calls
		mark(element, true).
		*/
		void mark(const Element_ConstIterator& element)
		{
			mark(element, true);
		}

		//! Unmarks an element of a set.
		/*!
		This is a convenience function that calls
		mark(element, false).
		*/
		void unmark(const Element_ConstIterator& element)
		{
			mark(element, false);
		}

		//! Marks or unmarks an element of a set.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void mark(const Element_ConstIterator& element, bool markIt)
		{
			PENSURE(element != elementSet_.cend());

			if (element->marked() == markIt)
			{
				// Nothing to do.
				return;
			}

			Set_Iterator set = element->set_;
			if (markIt)
			{
				// The element is unmarked and we
				// want to mark it.

				if (set->marked() == 0)
				{
					// This is the first marked element
					// of the set. Add the set to be
					// splitted later.
					Split_Iterator split =
						splitSet_.emplace(
						splitSet_.cend(),
						set);
					
					set->split_ = split;
				}

				set->moveToMarked(cast(element));
			}
			else
			{
				// The element is marked and we
				// want to unmark it.

				set->moveToUnmarked(cast(element));

				if (set->marked() == 0)
				{
					// The set no more contains a marked
					// element. Remove the set from the
					// split set.
					splitSet_.erase(set->split_);
					set->split_ = splitSet_.end();
				}
			}
		}

		//! Splits sets with both marked and unmarked elements.
		/*!
		Time complexity:
		amortized constant

		Exception safety:
		nothrow
		*/
		void split()
		{
			while(!splitSet_.empty())
			{
				Set_Iterator set = splitSet_.back();
				splitSet_.pop_back();
				
				// As an invariant, a set is only pushed in the
				// splitSet if it has at least one marked element.
				// Enforce that.
				ASSERT_OP(set->marked(), >, 0);

				if (set->marked() == set->elements())
				{
					// If all elements have been marked,
					// there is nothing to split.
					set->shrinkToMarked();

					continue;
				}

				// This will contain the newly-created
				// set. The other part will reside in
				// the current set.
				Set_Iterator newSet;

				// Split off the smaller part, or the marked
				// part if of equal size.
				if (set->marked() <= set->unmarked())
				{
					// The marked part is the smaller part.
					// Make it the new set.
					newSet = setSet_.emplace(
						setSet_.cend(),
						Set(set->begin_, set->unmarkedBegin_, 
						splitSet_.end(), set->marked(),
						!set->type()));
					
					// Use the current set for the larger
					// unmarked part.
					set->shrinkToUnmarked();
				}
				else
				{
					// The unmarked part is the smaller part.
					// Make it the new set->
					newSet = setSet_.emplace(
						setSet_.cend(),
						Set(set->unmarkedBegin_, cast(set->end()),
						splitSet_.end(), set->unmarked(),
						set->type()));

					// Use the current set for the larger 
					// marked part.
					set->shrinkToMarked();
				}

				// Update the set-reference for each element
				// of the new set.
				newSet->updateElements(newSet);
			}
		}

		// Sets

		Set_ConstIterator setBegin() const
		{
			return setSet_.cbegin();
		}

		Set_ConstIterator setEnd() const
		{
			return setSet_.cend();
		}

		integer sets() const
		{
			return setSet_.size();
		}

		// Elements

		Element_ConstIterator elementBegin() const
		{
			return elementSet_.cbegin();
		}

		Element_ConstIterator elementEnd() const
		{
			return elementSet_.cend();
		}

		integer elements() const
		{
			return elementSet_.size();
		}

		// Splits

		Split_ConstIterator splitBegin() const
		{
			return splitSet_.cbegin();
		}

		Split_ConstIterator splitEnd() const
		{
			return splitSet_.cend();
		}

		integer splits() const
		{
			return splitSet_.size();
		}

	private:
		// TODO: Implement
		RefinablePartition(const RefinablePartition& that);

		//! Casts away the constness of an element iterator.
		Element_Iterator cast(const Element_ConstIterator& that)
		{
			return elementSet_.erase(that, that);
		}

		//! Casts away the constness of a set iterator.
		Set_Iterator cast(const Set_ConstIterator& that)
		{
			return setSet_.erase(that, that);
		}

		//! Casts away the constness of a split iterator.
		Split_Iterator cast(const Split_ConstIterator& that)
		{
			return splitSet_.erase(that, that);
		}

		//! Casts away the constness of a member iterator.
		Member_Iterator cast(const Member_ConstIterator& that)
		{
			return memberSet_.erase(that, that);
		}

		//! The set of elements.
		ElementSet elementSet_;

		//! The set of sets.
		SetSet setSet_;

		//! The partition of elements.
		MemberSet memberSet_;

		//! Sets with marked elements.
		SplitSet splitSet_;
	};

}

#endif
