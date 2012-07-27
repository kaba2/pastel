// Description: Refinable partition

#ifndef PASTEL_REFINABLE_PARTITION_H
#define PASTEL_REFINABLE_PARTITION_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/refinable_partition_fwd.h"
#include "pastel/sys/refinable_partition_element.h"
#include "pastel/sys/refinable_partition_set.h"

#include <iterator>

namespace Pastel
{

	//! Refinable partition
	/*!
	Preconditions:
	ElementData is move/copy-constructible.
	SetData is move/copy-constructible.

	Note:
	Specifying ElementData or SetData as EmptyClass avoids 
	allocating any memory for the data.
	*/
	template <
		typename ElementData = void, 
		typename SetData = void>
	class RefinablePartition
		: public RefinablePartition_Fwd<ElementData, SetData>
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

		//! Copy-constructs from another partition.
		/*!
		Time complexity:
		O(that.sets() + that.elements())

		Exception safety:
		strong
		*/
		RefinablePartition(const RefinablePartition& that)
			: elementSet_()
			, setSet_()
			, memberSet_()
			, splitSet_()
		{
			RefinablePartition copy;

			// For each set...
			for (Set_ConstIterator set = that.cSetBegin();
				set != that.cSetEnd();
				++set)
			{
				// Create a copy-set.
				Set_Iterator copySet = copy.addSet(*set);

				// Then copy the elements from the
				// set to the copy-set.
				for (Member_ConstIterator member = set->begin();
					member != set->end();
					++member)
				{
					Element_Iterator element = *member;
					Element_Iterator copyElement = 
						copy.insertOne(copySet, *element);
					if (element->marked())
					{
						// If the element is marked,
						// mark the copied element too.
						copy.mark(copyElement);
					}
				}
			}

			swap(copy);
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
		Set_Iterator addSet(
			SetData setData = SetData())
		{
			return setSet_.emplace(
				setSet_.cend(), 
				Set(
				memberSet_.end(),
				memberSet_.end(),
				splitSet_.end(),
				0, false,
				std::move(setData)));
		}

		//! Inserts a set with the given elements into the partition.
		/*!
		Time complexity:
		O(std::distance(begin, end))

		Exception safety:
		strong
		*/
		template <typename ElementData_ConstIterator>
		Set_Iterator addSet(
			const ElementData_ConstIterator& begin,
			const ElementData_ConstIterator& end,
			SetData setData = SetData())
		{
			Set_Iterator set =
				addSet(std::move(setData));
			
			try
			{
				insert(set, begin, end);
			}
			catch(...)
			{
				erase(set);
				throw;
			}

			return set;
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
		Set_Iterator erase(
			const Set_ConstIterator& set)
		{
			// Remove all the elements and members
			// of the set.
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

		//! Splits sets with both marked and unmarked elements.
		/*!
		Time complexity: 
		amortized constant

		Exception safety: 
		nothrow
		*/
		void split(SetData setData = SetData())
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
						!set->type(), std::move(setData)));
					
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
						set->type(),
						std::move(setData)));

					// Use the current set for the larger 
					// marked part.
					set->shrinkToMarked();
				}

				// Update the set-reference for each element
				// of the new set.
				newSet->updateElements(newSet);
			}
		}

		// Returns the first iterator of the set-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_Iterator setBegin()
		{
			return setSet_.begin();
		}

		// Returns the first iterator of the set-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_ConstIterator cSetBegin() const
		{
			return setSet_.cbegin();
		}

		// Returns the end-iterator of the set-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_Iterator setEnd()
		{
			return setSet_.end();
		}

		// Returns the end-iterator of the set-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_ConstIterator cSetEnd() const
		{
			return setSet_.cend();
		}

		//! Returns the number of sets in the partition.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer sets() const
		{
			return setSet_.size();
		}

		//! Casts away the constness of a set iterator.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Set_Iterator cast(const Set_ConstIterator& that)
		{
			return setSet_.erase(that, that);
		}

		// Elements

		//! Inserts an element into a set.
		/*!
		Preconditions:
		set != cSetEnd()

		Time complexity: constant
		Exception safety: strong
		*/
		Element_Iterator insertOne(
			const Set_ConstIterator& set,
			ElementData elementData)
		{
			ENSURE(set != cSetEnd());

			Element_Iterator element =
				elementSet_.emplace(
				elementSet_.cend(),
				Element(cast(set), 
				memberSet_.end(),
				std::move(elementData)));

			try
			{
				Member_Iterator member =
					memberSet_.emplace(
					set->unmarkedEnd(),
					element);

				element->member_ = member;
				cast(set)->insertUnmarked(element);
			}
			catch(...)
			{
				elementSet_.pop_back();
				throw;
			};

			return element;
		}

		//! Inserts elements into a set.
		/*!
		Preconditions:
		set != cSetEnd()

		returns:
		An iterator to the set into which the elements
		were inserted.

		Time complexity:
		O(std::distance(begin, end))
		
		Exception safety:
		strong
		*/
		template <typename ElementData_ConstIterator>
		void insert(
			const Set_ConstIterator& set,
			const ElementData_ConstIterator& begin,
			const ElementData_ConstIterator& end)
		{
			ENSURE(set != cSetEnd());

			if (begin == end)
			{
				// Nothing to do.
				return;
			}

			// Create the elements and the initial partition.
			integer elements = 0;
			for (auto iter = begin; iter != end; ++iter)
			{
				try
				{
					insertOne(set, *iter);
					++elements;
				}
				catch(...)
				{
					// Roll back element creation.
					for (integer i = 0;i < elements;++i)
					{
						elementSet_.pop_back();
						memberSet_.pop_back();
					}

					// Roll back set creation.
					erase(set);

					throw;
				}
			}
		}

		//! Removes an element from the partition.
		/*!
		returns:
		std::next(element)

		Note:
		If the containing set of the element becomes empty,
		the set is not removed.
		*/
		Element_Iterator erase(
			const Element_ConstIterator& element)
		{
			// If the element is marked, unmark it.
			mark(element, false);

			// Remove the element from its set.
			cast(element)->set_->erase(
				cast(element),
				memberSet_.end());

			// Remove the element from the member-set.
			memberSet_.erase(element->member());

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
		markIt:
		Whether to mark the element (true)
		or unmark the element (false).

		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void mark(
			const Element_ConstIterator& element, 
			bool markIt)
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

		// Returns the first iterator of the element-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Element_Iterator elementBegin()
		{
			return elementSet_.begin();
		}

		// Returns the first iterator of the element-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Element_ConstIterator cElementBegin() const
		{
			return elementSet_.cbegin();
		}

		// Returns the end-iterator of the element-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Element_Iterator elementEnd()
		{
			return elementSet_.end();
		}

		// Returns the end-iterator of the element-set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Element_ConstIterator cElementEnd() const
		{
			return elementSet_.cend();
		}

		//! Returns the number of elements.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer elements() const
		{
			return elementSet_.size();
		}

		//! Casts away the constness of an element iterator.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Element_Iterator cast(const Element_ConstIterator& that)
		{
			return elementSet_.erase(that, that);
		}

		// Splits

		//! Returns the first iterator of the marked-set set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Split_ConstIterator splitBegin() const
		{
			return splitSet_.cbegin();
		}

		//! Returns the end-iterator of the marked-set set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Split_ConstIterator splitEnd() const
		{
			return splitSet_.cend();
		}

		//! Returns the number of marked sets.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer splits() const
		{
			return splitSet_.size();
		}

	private:
		//! Casts away the constness of a member iterator.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_Iterator cast(const Member_ConstIterator& that)
		{
			return memberSet_.erase(that, that);
		}

		//! Casts away the constness of a split iterator.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Split_Iterator cast(const Split_ConstIterator& that)
		{
			return splitSet_.erase(that, that);
		}

		//! The set of elements.
		ElementSet elementSet_;

		//! The set of sets.
		SetSet setSet_;

		//! The partition of elements.
		/*!
		The partition is stored in this list such that each
		set has an associated interval to this list. The
		elements in this interval are the members of the
		set. The interval may be empty.
		*/
		MemberSet memberSet_;

		//! Sets with marked elements.
		/*!
		This set contains exactly those sets of the partition 
		which have at least one marked element. The split() 
		function goes through this list and splits all of 
		these sets (except those which have all of their 
		elements marked).
		*/
		SplitSet splitSet_;
	};

}

#endif
