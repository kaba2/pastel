#ifndef PASTEL_REFINABLE_PARTITION_SET_H
#define PASTEL_REFINABLE_PARTITION_SET_H

#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	template <
		typename ElementData,
		typename SetData>
	class RefinablePartition_Fwd<ElementData, SetData>::Set
		: public PossiblyEmptyMember<SetData>
	{
	public:
		typedef PossiblyEmptyMember<SetData> Data;

		//! Move-constructs from another set.
		/*!
		Time complexity: constant
		Exception safety: strong

		FIX: This function is needed solely because Visual Studio 2010
		does not support the emplace function properly. Remove this 
		function when support for emplace becomes available.
		*/
		Set(Set&& that)
			: begin_(that.begin_)
			, last_(that.last_)
			, unmarkedBegin_(that.unmarkedBegin_)
			, split_(that.split_)
			, elements_(that.elements_)
			, marked_(that.marked_)
			, type_(that.type_)
		{
			if (Data::data())
			{
				new(Data::data()) SetData(std::move(that.data()));
			}
		}

		//! Destructs the set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~Set()
		{
			if (Data::data())
			{
				Data::data()->~SetData();
			}
		}

		//! Returns the contained data.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		SetData& data()
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the contained data.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		const SetData& data() const
		{
			PENSURE(Data::data());
			return *Data::data();
		}

		//! Returns the number of elements.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer elements() const
		{
			return elements_;
		}

		//! Returns the number of marked elements.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer marked() const
		{
			return marked_;
		}

		//! Returns the number of unmarked elements.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer unmarked() const
		{
			return elements_ - marked_;
		}

		//! Returns the first iterator of the members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator begin() const
		{
			return begin_;
		}

		//! Returns the end-iterator of the members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator end() const
		{
			return elements() > 0 ? std::next(last_) : last_;
		}

		//! Returns the first iterator of the unmarked members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator unmarkedBegin() const
		{
			return unmarkedBegin_;
		}

		//! Returns the end-iterator of the unmarked members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator unmarkedEnd() const
		{
			return end();
		}

		//! Returns the first iterator of the marked members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator markedBegin() const
		{
			return begin();
		}

		//! Returns the end-iterator of the marked members.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		Member_ConstIterator markedEnd() const
		{
			return unmarkedBegin_;
		}

	private:
		template <typename ElementData, typename SetData>
		friend class RefinablePartition;

		template <typename ElementData, typename SetData>
		friend class RefinablePartition<ElementData, SetData>::Element;

		// Deleted.
		Set();

		// Deleted.
		Set(const Set& that);

		// Deleted.
		Set& operator=(Set that);

		Set(Member_Iterator begin,
			Member_Iterator end,
			Split_Iterator split,
			integer elements,
			bool type,
			SetData data)
			: begin_(begin)
			, last_(elements > 0 ? std::prev(end) : end)
			, unmarkedBegin_(begin)
			, split_(split)
			, elements_(elements)
			, marked_(0)
			, type_(type)
		{
			if (Data::data())
			{
				new(Data::data()) SetData(std::move(data));
			}
		}

		//! Returns the type of the set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool type() const
		{
			return type_;
		}

		//! Sets the member-set interval.
		/*!
		Preconditions:
		elements > 0

		Time complexity: constant
		Exception safety: nothrow
		*/
		void set(
			Member_Iterator begin,
			Member_Iterator end,
			integer elements)
		{
			ASSERT_OP(elements, >=, 0);

			begin_ = begin;
			last_ = elements > 0 ? std::prev(end) : end;
			unmarkedBegin_ = begin_;
			elements_ = elements;
			marked_ = 0;
		}

		//! Forgets the elements in the unmarked part.
		/*!
		Preconditions:
		marked() > 0

		Time complexity: constant
		Exception safety: nothrow
		*/
		void shrinkToMarked()
		{
			ASSERT_OP(marked_, >, 0);

			if (marked_ < elements_)
			{
				// Shrink the set to the marked
				// elements.
				last_ = std::prev(unmarkedBegin_);
				elements_ = marked_;
			}
			
			// Switch the type of the set
			// so that the elements need not
			// change their type to get
			// unmarked.
			type_ = !type_;
			
			// Remove marked elements.
			unmarkedBegin_ = begin_;
			marked_ = 0;
		}

		//! Forgets the elements in the marked part.
		/*!
		Preconditions:
		marked() < elements()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void shrinkToUnmarked()
		{
			ASSERT_OP(marked_, <, elements_);

			// Shrink the set to the unmarked
			// elements.
			begin_ = unmarkedBegin_;
			elements_ -= marked_;
			
			// Remove marked elements.
			unmarkedBegin_ = begin_;
			marked_ = 0;
		}

		//! Moves an element from unmarked to marked part.
		/*!
		Preconditions:
		&*element->set() == this
		!element->marked()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void moveToMarked(
			const Element_Iterator& element)
		{
			ASSERT(&*element->set() == this);
			ASSERT(!element->marked());

			Element_Iterator& p = *element->member_;
			Element_Iterator& q = *unmarkedBegin_;

			// Swap the first unmarked element with
			// the element to be marked.
			std::swap(p, q);
			std::swap(p->member_, q->member_);

			// Then extend the marked region to
			// cover the given element.
			++unmarkedBegin_;
			++marked_;

			// Mark the element.
			element->mark(true);
		}

		//! Moves an element from marked to unmarked part.
		/*!
		Preconditions:
		&*element->set_ == this
		element->marked()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void moveToUnmarked(
			const Element_Iterator& element)
		{
			ASSERT(&*element->set_ == this);
			ASSERT(element->marked());

			Element_Iterator& p = *element->member_;
			Element_Iterator& q = *unmarkedBegin_;

			// Swap the last marked element with
			// the element to be unmarked.
			std::swap(p, std::prev(q));
			std::swap(p->member_, std::prev(q)->member_);

			// Then shrink the marked region to
			// exclude the given element.
			--unmarkedBegin_;
			--marked_;

			// Unmark the element.
			element->mark(false);
		}

		//! Updates the element's set-reference.
		/*!
		Preconditions:
		&*set == this

		Time complexity: O(set->elements())
		Exception safety: nothrow
		*/
		void updateElements(
			Set_Iterator set)
		{
			ASSERT(&*set == this);

			auto memberEnd = end();
			for (auto member = begin_; 
				member != memberEnd; 
				++member)
			{
				(*member)->set_ = set;
				ASSERT((*member)->type_ == type_);
			}
		}

		//! Removes an element from the set.
		/*!
		Preconditions:
		&*element->set_ == this
		!element->marked()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void erase(
			const Element_ConstIterator& element,
			const Member_Iterator& memberEnd)
		{
			ASSERT(&*element->set_ == this);
			ASSERT(!element->marked());

			Member_Iterator member =
				element->member_;
			
			--elements_;
			if (elements_ == 0)
			{
				begin_ = memberEnd;
				last_ = memberEnd;
				unmarkedBegin_ = begin_;
			}
			else if (member == begin_)
			{
				++begin_;
			}
			else if (member == last_)
			{
				--last_;
			}
		}

		//! The first iterator of the member-set.
		Member_Iterator begin_;

		//! The last iterator of the member-set.
		/*!
		We need to store the last iterator rather than
		the end-iterator, because otherwise removing a
		set could invalidate the end-iterator of another
		set. In elements_ = 0, then begin_ = last_ = 
		unmarkedBegin_ = end-iterator.
		*/
		Member_Iterator last_;

		//! The first iterator of the unmarked member-set.
		Member_Iterator unmarkedBegin_;

		//! If the set is in the split set, an iterator to that.
		/*!
		This iterator is needed so that when the number of
		marked elements goes to zero, the set can be removed
		from the split set in constant time.
		*/
		Split_Iterator split_;

		//! The number of elements in the set.
		/*!
		This number needs to be tracked because all the
		iterators are bidirectional.
		*/
		integer elements_;

		//! The number of marked elements in the set.
		/*!
		This number needs to be tracked because all the
		iterators are bidirectional.
		*/
		integer marked_;

		//! The type of the set.
		/*!
		An element is marked if and only if its type 
		differs from the type of its containing set.
		*/
		bool type_;
	};

}

#endif
