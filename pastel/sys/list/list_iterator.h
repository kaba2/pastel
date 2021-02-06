// Description: List iterator

#ifndef PASTELSYS_LIST_ITERATOR_H
#define PASTELSYS_LIST_ITERATOR_H

#include "pastel/sys/list.h"
#include "pastel/sys/list/list_node.h"

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
			typename std::conditional<
				std::is_const<
				typename std::remove_pointer<typename Settings::NodePtr>::type>::value,
				const typename Settings::Node_Settings::Data,
				typename Settings::Node_Settings::Data>::type,
			boost::bidirectional_traversal_tag>
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Node_Settings);
			PASTEL_FWD(NodePtr);

			using Data = typename Node_Settings::Data;
			using EndData = typename Node_Settings::EndData;
			using EndBase = typename Node_Settings::EndBase;
			static constexpr bool UserDataInEndNode =
				Settings::UserDataInEndNode;

			template <typename That_Settings>
			class IsConvertible
			{
			public:
				using That_Node_Settings = typename That_Settings::Node_Settings;

				static constexpr bool value = 
					std::is_convertible<typename That_Settings::NodePtr, NodePtr>::value &&
					std::is_same<typename That_Node_Settings::Data, Data>::value &&
					std::is_same<typename That_Node_Settings::EndData, EndData>::value;
			};

			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			template <typename That_Settings>
			requires (IsConvertible<That_Settings>::value)
			Iterator(const Iterator<That_Settings>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			Iterator& operator=(const Iterator& that)
			{
				this->base_reference() = that.base();
				return *this;
			}

			using DataRef = typename std::conditional<
				std::is_const<typename std::remove_pointer<NodePtr>::type>::value,
				const Data&,
				Data&>::type;

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
				return ((Data_Node<Data>*)node())->data();
			}

			//! Returns the end-data.
			/*!
			Preconditions:
			isEnd()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			EndData& endData() const
			{
				PENSURE(isEnd());
				return ((End_Node<EndBase, EndData>*)node())->data();
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
				this->base_reference() = 0;
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
			bool operator==(
				const Iterator<That_Settings>& right) const
			{
				return this->base() == right.base();
			}

			template <typename That_Settings>
			bool operator!=(
				const Iterator<That_Settings>& right) const
			{
				return this->base() != right.base();
			}

			// FIX: Make private
			explicit Iterator(NodePtr that)
				: Iterator::iterator_adaptor_(that) 
			{
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::List;

			friend class boost::iterator_core_access;

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
