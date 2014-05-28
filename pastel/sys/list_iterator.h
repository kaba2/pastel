// Description: List iterator

#ifndef PASTELSYS_LIST_ITERATOR_H
#define PASTELSYS_LIST_ITERATOR_H

#include "pastel/sys/list.h"
#include "pastel/sys/list_node.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace List_
	{

		template <typename Settings>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<Settings>, 
			typename Settings::NodePtr,
			std::conditional_t<
				std::is_const<std::remove_pointer_t<typename Settings::NodePtr>>::value,
				const typename Settings::Node_Settings::Data_Class&,
				typename Settings::Node_Settings::Data_Class&>,
			boost::bidirectional_traversal_tag>
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Node_Settings);
			PASTEL_FWD(NodePtr);

			using Data_Class = typename Node_Settings::Data_Class;
			using EndData_Class = typename Node_Settings::EndData_Class;
			using EndBase = typename Node_Settings::EndBase;
			static PASTEL_CONSTEXPR bool UserDataInEndNode =
				Settings::UserDataInEndNode;

			template <typename That_Settings>
			class IsConvertible
			{
			public:
				using That_Node_Settings = typename That_Settings::Node_Settings;

				static PASTEL_CONSTEXPR bool value = 
					std::is_convertible<typename That_Settings::NodePtr, NodePtr>::value &&
					std::is_same<typename That_Node_Settings::Data_Class, Data_Class>::value &&
					std::is_same<typename That_Node_Settings::EndData_Class, EndData_Class>::value;
			};

			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			template <
				typename That_Settings,
				typename = PASTEL_ENABLE_IF_C(IsConvertible<That_Settings>::value, void)>
			Iterator(const Iterator<That_Settings>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			Iterator& operator=(const Iterator& that)
			{
				base_reference() = that.base();
				return *this;
			}

			using DataRef = std::conditional_t<
				std::is_const<std::remove_pointer_t<NodePtr>>::value,
				const Data_Class&,
				Data_Class&>;

			//! Returns the data of the node.
			/*!
			Preconditions:
			isNormal()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			DataRef data() const
			{
				PENSURE(!empty());
				PENSURE(UserDataInEndNode || !isEnd());
				return *((Data_Node<Data_Class>*)node());
			}

			//! Returns the end-data.
			/*!
			Preconditions:
			isEnd()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			EndData_Class& endData() const
			{
				PENSURE(isEnd());
				return *((End_Node<EndBase, EndData_Class>*)node());
			}

			//! Returns whether this is a first iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool isBegin() const
			{
				return !empty() &&
					(node()->prev() == node() ||
					node()->prev()->next() != node());
			}

			//! Returns whether this is an end iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool isEnd() const
			{
				return !empty() &&
					node()->next() == node();
			}

			//! Returns whether this is a normal iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow

			An iterator is normal if it is not null, and
			not a end.
			*/
			bool isNormal() const
			{
				return !empty() && !isEnd();
			}

			//! Returns whether this is a null iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow

			A default-constructed iterator is a null-iterator.
			*/
			bool empty() const
			{
				return node() == 0;
			}

			//! Sets the iterator to null.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			void clear()
			{
				base_reference() = 0;
			}

			//! Returns the next element.
			/*!
			Preconditions:
			!empty()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator next() const
			{
				PENSURE(!empty());
				return Iterator(node()->next());
			}

			//! Returns the previous element.
			/*!
			Preconditions:
			!empty()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator prev() const
			{
				PENSURE(!empty());
				return Iterator(node()->prev());
			}

			//! Returns the next/previous element.
			/*!
			Preconditions:
			!empty()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator next(bool right) const
			{
				PENSURE(!empty());
				return Iterator(node()->next(right));
			}

			template <typename That_Settings>
			bool operator<(
				const Iterator<That_Settings>& right) const
			{
				return base() < right.base();
			}

			template <typename That_Settings>
			bool operator>(
				const Iterator<That_Settings>& right) const
			{
				return base() > right.base();
			}

			template <typename That_Settings>
			bool operator<=(
				const Iterator<That_Settings>& right) const
			{
				return base() <= right.base();
			}

			template <typename That_Settings>
			bool operator>=(
				const Iterator<That_Settings>& right) const
			{
				return base() >= right.base();
			}

			template <typename That_Settings>
			bool operator==(
				const Iterator<That_Settings>& right) const
			{
				return base() == right.base();
			}

			template <typename That_Settings>
			bool operator!=(
				const Iterator<That_Settings>& right) const
			{
				return base() != right.base();
			}

		private:
			template <typename, template <typename> class>
			friend class List;

			friend class boost::iterator_core_access;

			explicit Iterator(NodePtr that)
				: Iterator::iterator_adaptor_(that) 
			{
			}

			NodePtr node() const
			{
				return this->base();
			}

			DataRef dereference() const
			{
				return data();
			}

			template <bool Right>
			void increment_()
			{
				if (empty())
				{
					return;
				}

				this->base_reference() = node()->next(Right);
			}

			void increment() 
			{ 
				increment_<true>();
			}

			void decrement() 
			{ 
				increment_<false>();
			}
		};
	
	}

}

namespace std
{

	template <typename Settings>
	struct hash<Pastel::List_::Iterator<Settings>>
	{
		Pastel::hash_integer operator()(
			const Pastel::List_::Iterator<Settings>& that) const
		{
			return Pastel::computeHash<typename Settings::NodePtr>(that.base());
		}
	};

}

#endif
