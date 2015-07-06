// Description: Doubly-linked list

#ifndef PASTELSYS_LIST_H
#define PASTELSYS_LIST_H

#include "pastel/sys/ensure.h"

#include "pastel/sys/list/list_concepts.h"
#include "pastel/sys/list/list_node.h"
#include "pastel/sys/list/list_iterator.h"

#include <iterator>
#include <functional>

#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>

namespace Pastel
{

	//! Doubly-linked list
	/*!
	Space complexity: O(size())
	*/
	template <
		typename Settings_,
		template <typename> class Customization_>
	class List
	: boost::equality_comparable<List<Settings_, Customization_>
	, boost::less_than_comparable<List<Settings_, Customization_>
	> >
	, public Customization_<Settings_>
	{
	public:
		using Settings = Settings_;
		using Fwd = List_Fwd<Settings>;

		PASTEL_FWD(Data_Class);
		
		PASTEL_FWD(Node);
		PASTEL_FWD(End_Node);
		PASTEL_FWD(Data_Node);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		PASTEL_FWD(iterator);
		PASTEL_FWD(const_iterator);

		//! Constructs an empty list.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		List()
		{
			this->onConstruction();
		}

		//! Constructs from an initializer-list.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		template <typename Type>
		List(std::initializer_list<Type> thatSet)
		{
			for (auto&& that : thatSet)
			{
				insertBack(that);
			}
			
			this->onConstruction();
		}

		//! Copy-constructs from another list.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong
		*/
		List(const List& that)
		{
			for (auto&& element : that)
			{
				insertBack(element);
			}
			
			this->onConstruction();
		}

		//! Move-constructs from another list.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		List(List&& that)
		{
			splice(end(), that);
			
			this->onConstruction();
		}

		//! Destructs the list.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~List()
		{
			clear();
			delete end_;
		}

		//! Copy-assigns from an initializer list.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong

		Preserves the end-iterator.
		*/
		template <typename Type>
		List& operator=(std::initializer_list<Type> that)
		{
			List copy(that);
			return (*this = std::move(copy));
		}

		//! Copy-assigns from another list.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong

		Preserves the end-iterator.
		*/
		List& operator=(const List& that)
		{
			return (*this = List(that));
		}

		//! Move-assigns from another list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		Preserves the end-iterator.
		*/
		List& operator=(List&& that)
		{
			clear();
			splice(end(), that);
			return *this;
		}

		//! Swaps two lists.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(List& that)
		{
			std::swap(begin_, that.begin_);
			std::swap(end_, that.end_);
			std::swap(size_, that.size_);
		}

		friend void swap(List& left, List& right)
		{
			left.swap(right);
		}

		//! Compares two lists for equality.
		/*!
		Time complexity: 
		O(1), if size() != that.size()
		O(size()), otherwise.

		Exception safety: nothrow

		Two lists are equal if they are of the same
		size and they contain equivalent elements in
		the same order.
		*/
		bool operator==(const List& that) const
		{
			if (size() != that.size())
			{
				return false;
			}

			return std::equal(begin(), end(), that.begin());
		}

		//! Compares two lists lexicographically.
		/*!
		Time complexity: 
		O(1), if size() != that.size()
		O(size()), otherwise

		Exception safety: nothrow
		*/
		bool operator<(const List& that) const
		{
			if (size() != that.size())
			{
				return size() < that.size();
			}

			return std::lexicographical_compare(
				begin(), end(),
				that.begin(), that.end());			
		}

		PASTEL_ITERATOR_FUNCTIONS(begin, nodeBegin());
		PASTEL_ITERATOR_FUNCTIONS(end, nodeEnd());
		PASTEL_ITERATOR_FUNCTIONS(last, std::prev(end()));
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns whether the list is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return (begin() == end());
		}

		//! Returns the number of elements in the list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns a reference to the first element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Data_Class& front() const
		{
			return *cbegin();
		}

		Data_Class& front()
		{
			return removeConst(addConst(*this).front());
		}

		//! Returns a reference to the last element.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		const Data_Class& back() const
		{
			return *clast();
		}

		Data_Class& back()
		{
			return removeConst(addConst(*this).back());
		}

		//! Inserts an element before the given element.
		/*!
		Preconditions:
		'there' e [begin(), end()]

		Time complexity: O(1)
		Exception safety: strong
		*/
		template <typename... Type>
		Iterator insert(
			const ConstIterator& there,
			Type&&... that);

		//! Inserts an element at the beginning of the list.
		/*!
		This is a convenience function which calls
		insert(begin(), std::forward<Type>(that)...).
		*/
		template <typename... Type>
		Iterator insertFront(
			Type&&... that)
		{
			return insert(begin(), std::forward<Type>(that)...);
		}

		//! Removes the first element of the list.
		/*!
		This is a convenience function which calls
		erase(begin()).
		*/
		Iterator eraseFront()
		{
			return erase(begin());
		}

		//! Inserts an element at the end of the list.
		/*!
		This is a convenience function which calls
		insert(end(), std::forward<Type>(that)...).
		*/
		template <typename... Type>
		Iterator insertBack(
			Type&&... that)
		{
			return insert(end(), std::forward<Type>(that)...);
		}

		//! Removes the last element of the list.
		/*!
		This is a convenience function which calls
		erase(last()).
		*/
		Iterator eraseBack()
		{
			return erase(last());
		}

		//! Removes an element in the given iterator position.
		/*!
		Preconditions:
		that.isNormal()
		that in [begin(), end()[

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes all elements in the given iterator range.
		/*!
		Preconditions:
		[begin, end[ subset [begin(), end()[

		Time complexity: O(std::distance(begin, end))
		Exception safety: nothrow

		returns:
		end
		*/
		Iterator erase(
			const ConstIterator& begin, 
			const ConstIterator& end)
		{
			ConstIterator current = begin;
			while (current != end)
			{
				current = erase(current);
			}

			return cast(end);
		}

		//! Removes all elements of the list.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		void clear()
		{
			while(!empty())
			{
				eraseBack();
			}
		}

		//! Moves all elements in 'that' before 'to'.
		/*!
		Preconditions:
		to in [begin(), end()].

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		List& splice(
			const ConstIterator& to, 
			List& that)
		{
			return splice(to, that, 
				that.begin(), that.end(),
				that.size());
		}

		//! Moves an element from 'that' before 'to'.
		/*!
		Preconditions:
		to in [begin(), end()]
		thatFrom in [that.begin(), that.end()[

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		List& splice(
			const ConstIterator& to, 
			List& that,
			const ConstIterator& thatFrom)
		{
			return splice(to, that, 
				thatFrom, std::next(thatFrom),
				1);
		}

		//! Moves elements from 'fromSet' to before 'to'.
		/*!
		Preconditions:
		to in [begin(), end()]
		begin in [that.begin(), that.end()[
		end in [that.begin(), that.end()[

		Time complexity: 
		O(1), if this == &fromSet,
		O(std::distance(begin, end)), otherwise.

		Exception safety: nothrow
		*/
		List& splice(
			const ConstIterator& to, 
			List& fromSet,
			const ConstIterator& begin,
			const ConstIterator& end);

		//! Converts a const iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.base());
		}

	private:
		Node*& nodeBegin()
		{
			return begin_;
		}

		Node* nodeBegin() const
		{
			return begin_;
		}

		Node*& nodeEnd()
		{
			return (Node*&)end_;
		}

		Node* nodeEnd() const
		{
			return (Node*)end_;
		}

		//! Moves elements from 'fromSet' to before 'to'.
		/*!
		Preconditions:
		to in [begin(), end()]
		begin in [that.begin(), that.end()[
		end in [that.begin(), that.end()[

		Time complexity: O(1)
		Exception safety: nothrow

		count:
		Must equal std::distance(begin, end).
		*/
		List& splice(
			const ConstIterator& to, 
			List& fromSet,
			const ConstIterator& begin,
			const ConstIterator& end,
			integer count);

		template <typename... Type>
		Data_Node* nodeAllocate(Type&&... data)
		{
			return new Data_Node(std::forward<Type>(data)...);
		}

		void nodeDeallocate(const Data_Node* node)
		{
			ASSERT(node && !node->isEnd());
			delete node;
		}

		void linkNodes(Node* that, Node* nextThat)
		{
			ASSERT(that);
			ASSERT(nextThat);

			if (that != nodeEnd())
			{
				that->next() = nextThat;
			}
			else
			{
				nodeBegin() = nextThat;
			}

			nextThat->prev() = that;
		}

		//! The end-node of the list.
		/*!
		The end-node works as the end-iterator.
		It is identified locally by its next
		node pointing to itself.
		*/
		End_Node* end_ = new End_Node;

		//! The first node of the list.
		/*!
		The begin-node is identified locally 
		by the next node of the previous node 
		not pointing to itself.
		*/
		Node* begin_ = (Node*)end_;

		//! The number of elements in the list.
		integer size_ = 0;
	};

}

namespace Pastel
{

	template <
		typename Data_ = void, 
		typename EndData_ = void,
		bool UserDataInEndNode_ = false,
		template <typename> class Customization_ = Empty_List_Customization>
	using List_Set = List<
		List_Set_Settings<Data_, EndData_, UserDataInEndNode_>, 
		Customization_>;

}

#include "pastel/sys/list/list_erase.hpp"
#include "pastel/sys/list/list_insert.hpp"
#include "pastel/sys/list/list_splice.hpp"

#include "pastel/sys/list/list_invariants.h"
#include "pastel/sys/list/list_merge.h"
#include "pastel/sys/list/list_partition.h"
#include "pastel/sys/list/list_remove_if.h"
#include "pastel/sys/list/list_reverse.h"
#include "pastel/sys/list/list_sort.h"
#include "pastel/sys/list/list_unique.h"

#endif
