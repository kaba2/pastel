#ifndef PASTEL_REFINABLE_PARTITION_BLOCK_H
#define PASTEL_REFINABLE_PARTITION_BLOCK_H

#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	template <typename Type>
	class RefinablePartition_Fwd<Type>::Set
	{
	public:
		Set(
			Position_Iterator begin,
			Position_Iterator end,
			Split_Iterator split,
			integer elements,
			bool type)
			: begin_(begin)
			, last_(elements > 0 ? std::prev(end) : end)
			, unmarkedBegin_(begin)
			, split_(split)
			, elements_(elements)
			, marked_(0)
			, type_(type)
		{
		}

		Set(const Set& that)
			: begin_(that.begin_)
			, last_(that.last_)
			, unmarkedBegin_(that.unmarkedBegin_)
			, split_(that.split_)
			, elements_(that.elements_)
			, marked_(that.marked_)
			, type_(that.type_)
		{
		}

		Set(Set&& that)
			: begin_()
			, last_()
			, unmarkedBegin_()
			, split_()
			, elements_(0)
			, marked_(0)
			, type_(false)
		{
			swap(that);
		}

		integer elements() const
		{
			return elements_;
		}

		integer marked() const
		{
			return marked_;
		}

		integer unmarked() const
		{
			return elements_ - marked_;
		}

		Position_ConstIterator begin() const
		{
			return begin_;
		}

		Position_ConstIterator end() const
		{
			return elements() > 0 ? std::next(last_) : last_;
		}

		Position_ConstIterator unmarkedBegin() const
		{
			return unmarkedBegin_;
		}

		Position_ConstIterator unmarkedEnd() const
		{
			return end();
		}

		Position_ConstIterator markedBegin() const
		{
			return begin();
		}

		Position_ConstIterator markedEnd() const
		{
			return unmarkedBegin_;
		}

	private:
		template <typename Type>
		friend class RefinablePartition;

		template <typename Type>
		friend class RefinablePartition<Type>::Element;

		// Deleted.
		Set();

		// Deleted.
		Set& operator=(Set that);

		void swap(Set& that)
		{
			using std::swap;
			swap(begin_, that.begin_);
			swap(last_, that.last_);
			swap(unmarkedBegin_, that.unmarkedBegin_);
			swap(split_, that.split_);
			swap(elements_, that.elements_);
			swap(marked_, that.marked_);
			swap(type_, that.type_);
		}

		bool type() const
		{
			return type_;
		}

		void set(
			Position_Iterator begin,
			Position_Iterator end,
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
		void moveToMarked(
			const Element_Iterator& element)
		{
			ASSERT(&*element->set() == this);
			ASSERT(!element->marked());

			Element_Iterator& p = *element->position_;
			Element_Iterator& q = *unmarkedBegin_;

			// Swap the first unmarked element with
			// the element to be marked.
			std::swap(p, q);
			std::swap(p->position_, q->position_);

			// Then extend the marked region to
			// cover the given element.
			++unmarkedBegin_;
			++marked_;

			// Mark the element.
			element->mark(true);
		}

		//! Moves an element from marked to unmarked part.
		void moveToUnmarked(
			const Element_Iterator& element)
		{
			ASSERT(&*element->set_ == this);
			ASSERT(element->marked());

			Element_Iterator& p = *element->position_;
			Element_Iterator& q = *unmarkedBegin_;

			// Swap the last marked element with
			// the element to be unmarked.
			std::swap(p, std::prev(q));
			std::swap(p->position_, std::prev(q)->position_);

			// Then shrink the marked region to
			// exclude the given element.
			--unmarkedBegin_;
			--marked_;

			// Unmark the element.
			element->mark(false);
		}

		//! Updates the element's set-reference.
		void updateElements(
			Set_Iterator set)
		{
			ASSERT(&*set == this);

			auto positionEnd = end();
			for (auto position = begin_; 
				position != positionEnd; 
				++position)
			{
				(*position)->set_ = set;
				ASSERT((*position)->type_ == type_);
			}
		}

		//! Removes an element from the set.
		void erase(
			const Element_ConstIterator& element,
			const Position_Iterator& positionEnd)
		{
			ASSERT(&*element->set_ == this);

			Position_Iterator position =
				element->position_;
			
			--elements_;
			if (elements_ == 0)
			{
				begin_ = positionEnd;
				last_ = positionEnd;
				unmarkedBegin_ = begin_;
			}
			else
			{
				if (position == begin_)
				{
					++begin_;
				}
				if (position == unmarkedBegin_)
				{
					if (position == last_)
					{
						unmarkedBegin_ = positionEnd;
					}
					else
					{
						++unmarkedBegin_;
					}
				}
				if (position == last_)
				{
					--last_;
				}
			}

			if (element->marked())
			{
				--marked_;
			}	
		}

		Position_Iterator begin_;
		Position_Iterator last_;
		Position_Iterator unmarkedBegin_;
		Split_Iterator split_;
		integer elements_;
		integer marked_;
		bool type_;
	};

}

#endif
