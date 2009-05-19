/*!
\file
\brief A class for a doubly linked list using Pastel's allocators.
*/

#ifndef PASTEL_FASTLIST_H
#define PASTEL_FASTLIST_H

#include <iterator>
#include <functional>

#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>

namespace Pastel
{

	//! A doubly linked list using Pastel's allocators.

	template <
		typename Type,
		typename UniformAllocator>
	class FastList
		: boost::less_than_comparable<
		FastList<Type, UniformAllocator>
		, boost::equality_comparable<
		FastList<Type, UniformAllocator>
		> >
	{
	private:
		class ValueTag {};
		class RangeTag {};

		class Node
		{
		public:
			Node()
				: next_(0)
				, previous_(0)
			{
			}

			Node* next_;
			Node* previous_;

		private:
			// Prohibited
			Node(const Node&);
			// Prohibited
			Node& operator=(const Node&);
		};

		class DataNode
			: public Node
		{
		public:
			DataNode()
				: Node()
				, data_()
			{
			}

			explicit DataNode(const Type& data)
				: Node()
				, data_(data)
			{
			}

			Type data_;
		private:
			// Prohibited
			DataNode(const DataNode&);
			// Prohibited
			DataNode& operator=(const DataNode&);
		};

	public:
		/*
		enum
		{
			AllocatorUnitSize = sizeof(DataNode)
		};
		*/

		typedef boost::shared_ptr<UniformAllocator>
			allocator_ptr;
		typedef boost::shared_ptr<const UniformAllocator>
			const_allocator_ptr;

		class iterator;
		class const_iterator;

		typedef std::reverse_iterator<iterator>
			reverse_iterator;
		typedef std::reverse_iterator<const_iterator>
			const_reverse_iterator;

		typedef UniformAllocator allocator_type;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef Type value_type;
		typedef int size_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef size_type difference_type;

		//! Constructs an empty list.
		/*!
		Preconditions:
		allocatorPtr.get() == 0 || allocatorPtr->unitSize() >= sizeof(Node)

		Time complexity: constant
		Exception safety: strong
		*/
		explicit FastList(
			const allocator_ptr& allocatorPtr =
			allocator_ptr());

		//! Constructs a list with 'count' occurences of 'that'.
		/*!
		Preconditions:
		allocatorPtr.get() == 0 || allocatorPtr->unitSize() >= sizeof(Node)
		'count' >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		explicit FastList(size_type count,
			const value_type& that = value_type(),
			const allocator_ptr& allocatorPtr =
			allocator_ptr());

		//! Constructs a list from the given values in the range.
		/*!
		Preconditions:
		allocatorPtr.get() == 0 || allocatorPtr->unitSize() >= sizeof(Node)

		Time complexity: linear
		Exception safety: strong
		*/
		template <typename InputIterator>
		FastList(InputIterator first,
			InputIterator last,
			const allocator_ptr& allocatorPtr =
			allocator_ptr());

		//! Constructs with the values of another list.
		/*!
		Preconditions:
		allocatorPtr.get() == 0 || allocatorPtr->unitSize() >= sizeof(Node)

		Time complexity: linear
		Exception safety: strong
		*/
		FastList(const FastList& that,
			const allocator_ptr& allocatorPtr =
			allocator_ptr());

		//! Destructs the list.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		~FastList();

		//! Assigns another list.
		/*!
		Time complexity: linear
		Exception safety: strong
		*/
		FastList<Type, UniformAllocator>& operator=(
			const FastList& that);

		//! Compares two lists for equality.
		/*!
		Preconditions:
		Type::operator==() does not throw.

		Time complexity: linear
		Exception safety: nothrow

		Two lists are equal if they are of the same
		size and they contain the same elements in
		the same order.
		*/
		bool operator==(const FastList& that) const;

		//! Compares two lists lexicographically.
		/*!
		Preconditions:
		Type::operator<() does not throw.

		Time complexity: linear
		Exception safety: nothrow

		A lexicographical order is given by the following
		procedure: starting from the first element of each
		list, compare the elements for equality. If the elements
		are not equal, return the result of their ordering
		when comparing with the operator <. If the elements are
		equal, proceed to the next element in both lists and
		repeat. If the process gets into no conclusion once all
		elements of either list have been compared, then the
		shorter list is considered the less one. If the lists
		contain the same amount of elements, the lists
		are equal, and thus the result is false.
		*/
		bool operator<(const FastList& that) const;

		//! Assigns a range of values into the list.
		/*!
		Time complexity: linear
		Exception safety: strong

		The existing elements are cleared away after a
		succesful assignment.
		*/
		template <typename InputIterator>
		void assign(InputIterator first,
			InputIterator last);

		//! Assigns 'count' amount of 'that' into the list.
		/*!
		Preconditions:
		'count' >= 0

		Time complexity: linear
		Exception safety: strong

		The existing elements are cleared away.
		*/
		void assign(size_type count, const value_type& that);

		//! Returns an iterator to the beginning of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		iterator begin();

		//! Returns a constant iterator to the beginning of the list.
		/*!
		Time complexity: constant
		Exception safety nothrow
		*/
		const_iterator begin() const;

		//! Returns an iterator to the end of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		iterator end();

		//! Returns a constant iterator to the end of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		const_iterator end() const;

		//! Returns a reverse iterator to the end of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		reverse_iterator rbegin();

		//! Returns a reverse constant iterator to the end of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		const_reverse_iterator rbegin() const;

		//! Returns a reverse iterator to the beginning of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		reverse_iterator rend();

		//! Returns a reverse constant iterator to the beginning of the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		const_reverse_iterator rend() const;

		//! Returns true if the list is empty, false otherwise.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool empty() const;

		//! Returns the number of elements in the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		size_type size() const;

		//! Returns the maximum number of elements containable in the list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		size_type max_size() const;

		//! Sets the allocator.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void set_allocator(const allocator_ptr& allocatorPtr);

		//! Returns the allocatorPtr.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		typename FastList<Type, UniformAllocator>::allocator_ptr
			get_allocator() const;

		//! Resizes the list to contain 'newSize' elements.
		/*!
		Preconditions:
		'newSize' >= 0

		Time complexity: linear
		Exception safety: strong

		If the list is smaller than the required size,
		the list is extended with the 'value' elements
		until the list is of the required size.

		Note that the 'value' is passed by value. This is
		to handle the problem of passing on an element
		of this list and then resizing smaller so that
		that element is no longer contained in the list.
		*/
		void resize(size_type newSize, value_type value = value_type());

		//! Returns a reference to the first element.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		reference front();

		//! Returns a constant reference to the first element.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		const_reference front() const;

		//! Returns a reference to the last element.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		reference back();

		//! Returns a constant reference to the last element.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		const_reference back() const;

		//! Inserts an element to the beginning of the list.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		void push_front(const value_type& that);

		//! Removes the first element of the list.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void pop_front();

		//! Inserts an element to the end of the list.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		void push_back(const value_type& that);

		//! Removes the last element of the list.
		/*!
		Preconditions:
		!empty()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void pop_back();

		//! Inserts an element before the given iterator.
		/*!
		Preconditions:
		'there' e [begin(), end()]

		Time complexity: constant
		Exception safety: strong
		*/
		iterator insert(iterator there,
			const value_type& that);

		//! Inserts 'count' elements of 'that' before the given iterator.
		/*!
		Preconditions:
		'there' e [begin(), end()]
		count >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		void insert(iterator there, size_type count,
			const value_type& that);

		//! Inserts a range of values before the given iterator.
		/*!
		Preconditions:
		'there' e [begin(), end()]
		'there' !e [first, last[

		Time complexity: linear
		Exception safety: strong
		*/
		template <typename InputIterator>
		void insert(iterator there,
			InputIterator first,
			InputIterator last);

		//! Removes an element in the given iterator position.
		/*!
		Preconditions:
		'there' e [begin(), end()[

		Time complexity: constant
		Exception safety: nothrow
		*/
		iterator erase(iterator there);

		//! Removes all elements in the given iterator range.
		/*!
		Preconditions:
		[first, last[ is a subrange of [begin(), end()[

		Time complexity: linear
		Exception safety: nothrow
		*/
		iterator erase(iterator first, iterator last);

		//! Swaps two lists.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(FastList<Type, UniformAllocator>& that);

		//! Removes all elements of the list.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void clear();

		//! Removes all occurences of 'that'.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void remove(const value_type& that);

		//! Removes all elements that satisfy the given predicate.
		/*!
		Preconditions:
		Predicate::operator() does not throw.

		Time complexity: linear
		Exception safety: nothrow
		*/
		template <typename Predicate>
		void remove_if(Predicate predicate);

		//! Removes adjacent equal elements w.r.t operator==.
		/*!
		Preconditions:
		Type::operator==() does not throw.

		Time complexity: linear
		Exception safety: nothrow

		For equal elements, the first one in the list is retained.
		*/
		void unique();

		//! Removes adjacent equal elements w.r.t the given predicate.
		/*!
		Preconditions:
		BinaryPredicate::operator() does not throw.

		Time complexity: linear
		Exception safety: nothrow

		For equal elements, the first one in the list is retained.
		*/
		template <typename BinaryPredicate>
		void unique(BinaryPredicate binaryPredicate);

		/*!
		Preconditions:
		*that.get_allocator() == *get_allocator()
		'there' e [begin(), end()].

		Time complexity: constant
		Exception safety: nothrow
		*/
		void splice(iterator there, FastList& that);

		/*!
		Preconditions:
		*that.get_allocator() == *get_allocator()
		'there' e [begin(), end()]
		'i' e [that.begin(), that.end()[.

		Time complexity: constant
		Exception safety: nothrow
		*/
		void splice(iterator there, FastList& that,
			iterator i);

		/*!
		Preconditions:
		*that.get_allocator() == *get_allocator()
		'there' e [begin(), end()].
		'there' !e [first, last[
		[first, last[ is a subrange of [that.begin(), that.end()[.

		Time complexity: constant
		Exception safety: nothrow
		*/
		void splice(iterator there, FastList& that,
			iterator first, iterator last);

		//! Merges two lists w.r.t. operator<.
		/*!
		Preconditions:
		*that.get_allocator() == *get_allocator()

		Time complexity: linear
		Exception safety: strong

		The result of the merging is ordered, if both lists
		are ordered (w.r.t operator<). If there are equal
		elements in the lists, the ones in this list comes
		before those in 'that'.
		*/
		void merge(FastList& that);

		//! Merges two lists w.r.t. the given predicate.
		/*!
		Preconditions:
		*that.get_allocator() == *get_allocator()

		Time complexity: linear
		Exception safety: strong

		The result of the merging is ordered, if both lists
		are ordered (w.r.t the given predicate). If there are equal
		elements in the lists, the ones in this list comes
		before those in 'that'.
		*/
		template <typename Compare>
		void merge(FastList& that,
			Compare compare);

		//! Sorts the list w.r.t. the operator<.
		/*!
		Time complexity: O(n lg n)
		Exception safety: strong

		The sort is stable.
		*/
		void sort();

		//! Sorts the list w.r.t. the given predicate.
		/*!
		Time complexity: O(n lg n)
		Exception safety: strong

		The sort is stable.
		*/
		template <typename Compare>
		void sort(Compare compare);

		//! Reverses the list.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void reverse();

	private:
		void initialize();

		Node* nodeBegin();
		const Node* nodeBegin() const;
		Node* nodeEnd();
		const Node* nodeEnd() const;

		iterator insert(
			const iterator& there,
			size_type count,
			const value_type& data,
			ValueTag);

		template <typename InputIterator>
		iterator insert(
			const iterator& there,
			const InputIterator& from,
			const InputIterator& to,
			RangeTag);

		Node* insert(Node* node, const value_type& data);

		Node* erase(Node* node);

		void splice(Node* there, FastList& that,
			Node* thatFrom, Node* thatTo);

		DataNode* nodeAllocate(const value_type& data);
		void nodeDeAllocate(DataNode* node);

		void linkNodes(Node* that, Node* nextThat);
		void addSize(size_type count);
		// You can't use addSize(-count), because
		// you can also subtract UnknownSize.
		void subtractSize(size_type count);

		enum
		{
			UnknownSize = -1
		};

		Node* head_;
		mutable size_type size_;
		allocator_ptr allocator_;
	};

}

#include "pastel/sys/fastlist.hpp"

#endif
