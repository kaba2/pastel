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

		template <
			typename NodePtr,
			typename Node_Settings>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Node_Settings>, 
			NodePtr,
			std::conditional_t<
				std::is_const<std::remove_pointer_t<NodePtr>>::value,
				const typename Node_Settings::Data_Class&,
				typename Node_Settings::Data_Class&>,
			boost::bidirectional_traversal_tag>
		{
		public:
			using Fwd = Node_Settings;

			PASTEL_FWD(Data_Class);
			PASTEL_FWD(SentinelData_Class);

			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			template <
				typename That_NodePtr,
				typename = PASTEL_ENABLE_IF((boost::is_convertible<That_NodePtr, NodePtr>), void)>
			Iterator(
				const Iterator<That_NodePtr, Node_Settings>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
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
				PENSURE(isNormal());
				return *node();
			}

			//! Returns the sentinel data.
			/*!
			Preconditions:
			isSentinel()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			SentinelData_Class& sentinelData() const
			{
				PENSURE(isSentinel());
				return ((Sentinel_Node<Node_Settings>*)node())->sentinelData();
			}

			//! Returns whether this is a sentinel iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool isSentinel() const
			{
				if (empty())
				{
					return false;
				}

				return node()->next() == 0;
			}

			//! Returns whether this is a normal iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow

			An iterator is normal if it is not null, and
			not a sentinel.
			*/
			bool isNormal() const
			{
				return !empty() && !isSentinel();
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

				NodePtr& node_ = this->base_reference();
				node_ = node_->next(Right);
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

	template <
		typename NodePtr,
		typename Node_Settings>
	struct hash<Pastel::List_::Iterator<NodePtr, Node_Settings>>
	{
		Pastel::hash_integer operator()(
			const Pastel::List_::Iterator<NodePtr, Node_Settings>& that) const
		{
			return Pastel::computeHash<NodePtr>(that.base());
		}
	};

}

#endif
