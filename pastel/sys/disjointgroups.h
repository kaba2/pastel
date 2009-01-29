#ifndef PASTELSYS_DISJOINTGROUPS_H
#define PASTELSYS_DISJOINTGROUPS_H

#include "pastel/sys/unorderedset.h"

#include "pastel/sys/fastlist.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/possiblyemptymember.h"

namespace Pastel
{

	//! A data structure for disjoint sets.
	/*!
	We chose to name this class DisjointGroups, because
	set's are already used in STL. By choosing Type = EmptyClass,
	no memory is allocated for data. In this case
	calling Element::data() results in an assertion.

	An element can report it's group in constant time,
	but taking the union of two groups takes linear time.

	Disjoint-set forests seem to offer more efficient
	operations. However, it can't
	1) remove elements
	2) remove groups
	3) traverse over the elements of a group

	This implementation is able to do all of these.
	*/

	template <typename Type>
	class DisjointGroups
	{
	private:
		class Element;
		typedef FastList<Element, PoolAllocator> ElementContainer;

		class Group;
		typedef FastList<Group, PoolAllocator> GroupContainer;

	public:
		// Using default constructor.
		// Using default destructor.

		typedef typename GroupContainer::iterator GroupIterator;
		typedef typename GroupContainer::const_iterator ConstGroupIterator;

		typedef typename ElementContainer::iterator ElementIterator;
		typedef typename ElementContainer::const_iterator ConstElementIterator;

		// Elements

		//! Constructs an empty structure.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		DisjointGroups();

		//! Constructs a copy of another group.
		/*!
		Time complexity: linear
		Exception safety: strong
		*/
		DisjointGroups(const DisjointGroups& that);

		//! Copies another group.
		/*!
		Time complexity: linear
		Exception safety: strong
		*/
		DisjointGroups<Type>& operator=(const DisjointGroups& that);

		//! Swaps two disjoint groups.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(DisjointGroups& that);

		//! Removes all groups.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void clear();

		//! Returns an iterator to the beginning of the element list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ElementIterator elementBegin();

		//! Returns an iterator to the beginning of the element list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ConstElementIterator elementBegin() const;

		//! Returns an iterator to the end of the element list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ElementIterator elementEnd();

		//! Returns an iterator to the end of the element list.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ConstElementIterator elementEnd() const;

		//! Removes an element.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void erase(const ElementIterator& element);

		//! Returns the number of elements in all groups.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer elements() const;

		// Groups

		//! Inserts an empty group.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		GroupIterator insertGroup();

		//! Returns an iterator to the beginning of the group set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		GroupIterator groupBegin();

		//! Returns an iterator to the beginning of the group set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ConstGroupIterator groupBegin() const;

		//! Returns an iterator to the end of the group set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		GroupIterator groupEnd();

		//! Returns an iterator to the end of the group set.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		ConstGroupIterator groupEnd() const;

		//! Removes a group.
		/*!
		Time complexity: linear in the number of group elements
		Exception safety: nothrow
		*/
		void erase(const GroupIterator& group);

		//! Returns the number of groups.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer groups() const;

		//! Inserts an element into a group.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		ElementIterator insert(
			const GroupIterator& group,
			const Type& data = Type());

		//! Merges two groups into one.
		/*!
		Returns:
		An iterator to the union group.

		Note:
		Either 'aGroup' or 'bGroup' is
		destroyed in the process.
		It is not specified which one.

		Time complexity:
		linear in the number of elements of the smaller group
		Exception safety:
		nothrow
		*/
		GroupIterator merge(
			const GroupIterator& aGroup,
			const GroupIterator& bGroup);

	private:
		class Element
			: private PossiblyEmptyMember<Type>
		{
		public:
			friend class DisjointGroups<Type>;

			Element(
				const GroupIterator& group,
				const Type& data)
				: PossiblyEmptyMember<Type>()
				, group_(group)
			{
				Type* address = PossiblyEmptyMember<Type>::data();
				if (address)
				{
					new(address) Type(data);
				}
			}

			Element(const Element& that)
				: PossiblyEmptyMember<Type>()
				, group_(that.group_)
			{
				Type* address = PossiblyEmptyMember<Type>::data();
				if (address)
				{
					new(address) Type(that.data());
				}
			}

			void swap(Element& that)
			{
				Type* thisAddress = PossiblyEmptyMember<Type>::data();
				Type* thatAddress = ((PossiblyEmptyMember<Type>&)that).data();
				if (thisAddress && thatAddress)
				{
					data().swap(that.data());
				}
				group_.swap(that.group_);
			}

			Element& operator=(const Element& that)
			{
				Element copy(that);
				swap(copy);
				return *this;
			}

			~Element()
			{
				Type* address = PossiblyEmptyMember<Type>::data();
				if (address)
				{
					address->~Type();
				}
			}

			GroupIterator group()
			{
				return group_;
			}

			ConstGroupIterator group() const
			{
				return group_;
			}

			Type& data()
			{
				PENSURE(PossiblyEmptyMember<Type>::data());
				return *PossiblyEmptyMember<Type>::data();
			}

			const Type& data() const
			{
				PENSURE(PossiblyEmptyMember<Type>::data());
				return *PossiblyEmptyMember<Type>::data();
			}

		private:
			GroupIterator group_;
		};

		class Group
		{
		public:
			friend class DisjointGroups<Type>;

			explicit Group(
				const ElementIterator& first,
				const ElementIterator& last,
				integer size)
				: first_(first)
				, last_(last)
				, size_(size)
			{
			}

			bool empty() const
			{
				return size_ == 0;
			}

			integer size() const
			{
				return size_;
			}

			ElementIterator begin()
			{
				return first_;
			}

			ConstElementIterator begin() const
			{
				return first_;
			}

			ElementIterator end()
			{
				if (size_ == 0)
				{
					return last_;
				}

				ElementIterator result = last_;
				++result;

				return result;
			}

			ConstElementIterator end() const
			{
				if (size_ == 0)
				{
					return last_;
				}

				ConstElementIterator result = last_;
				++result;

				return result;
			}

		private:
			ElementIterator first_;
			ElementIterator last_;
			integer size_;
		};

		GroupContainer groupSet_;
		ElementContainer elementList_;
	};

}

#include "pastel/sys/disjointgroups.hpp"

#endif
