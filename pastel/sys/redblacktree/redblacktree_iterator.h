// Description: Red-black tree iterator

#ifndef PASTELSYS_REDBLACKTREE_ITERATOR_H
#define PASTELSYS_REDBLACKTREE_ITERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/redblacktree/redblacktree_fwd.h"
#include "pastel/sys/redblacktree/redblacktree_node.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/downfilter/all_downfilter.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <
			typename NodePtr,
			typename Node_Settings,
			bool DereferenceToData>
		class Iterator
			: public boost::iterator_adaptor<
			Iterator<NodePtr, Node_Settings, DereferenceToData>, 
			NodePtr,
			typename std::conditional<DereferenceToData, 
			typename Node_Settings::Data, 
			const typename Node_Settings::Key>::type,
			boost::bidirectional_traversal_tag>
		{
		public:
			using Fwd = Node_Settings;

			PASTEL_FWD(Key);
			PASTEL_FWD(Data);
			PASTEL_FWD(Propagation);
			PASTEL_FWD(SentinelData);
			static constexpr bool UserDataInSentinelNodes =
				Node_Settings::UserDataInSentinelNodes;

			template <bool DereferenceToData_>
			using Other_Iterator = Iterator<NodePtr, Node_Settings, DereferenceToData_>;

			using Key_Iterator = Other_Iterator<false>;
			using Data_Iterator = Other_Iterator<true>;

			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			template <
				typename That_NodePtr,
				bool That_DereferenceToData>
			requires std::is_convertible_v<That_NodePtr, NodePtr>
			Iterator(
				const Iterator<That_NodePtr, Node_Settings, That_DereferenceToData>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			//! Returns an iterator which dereferences to the key.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Key_Iterator dereferenceKey() const
			{
				return Key_Iterator(*this);
			}

			//! Returns an iterator which dereferences to user data.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Data_Iterator dereferenceData() const
			{
				return Data_Iterator(*this);
			}

			//! Returns the key of the node.
			/*!
			Preconditions:
			isNormal()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			const Key& key() const
			{
				PENSURE(isNormal());
				return ((Key_Node<Node_Settings>*)node())->key();
			}

			//! Returns the propagation data.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			const Propagation& propagation() const
			{
				return ((Propagation_Node<Node_Settings>*)node())->propagation();
			}

			//! Returns the user data.
			/*!
			Preconditions:
			!empty()
			UserDataInSentinelNodes || !isSentinel()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Data& data() const
			{
				PENSURE(!empty());
				PENSURE(UserDataInSentinelNodes || !isSentinel());
				return ((Data_Node<Node_Settings>*)node())->data();
			}

			//! Returns the sentinel data.
			/*!
			Preconditions:
			isSentinel()

			Time complexity: O(1)
			Exception safety: nothrow
			*/
			SentinelData& sentinelData() const
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
				return node()->isSentinel();
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

			//! Returns whether this node has valid size.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow

			This function is useful only for debugging.
			When the implementation is correct, this function
			always returns true.
			*/
			bool validPropagation() const
			{
				return node()->validPropagation();
			}

			//! Returns the parent node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator parent() const
			{
				return Iterator(node()->parent());
			}

			//! Returns the left child node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator left() const
			{
				return Iterator(node()->left());
			}

			//! Returns the right child node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator right() const
			{
				return Iterator(node()->right());
			}

			//! Returns the given child node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator child(bool right) const
			{
				return Iterator(node()->child(right));
			}

			//! Finds the next marked element.
			/*!
			Time complexity: 
			O(log(n))
			where
			n is the number of elements in the tree.

			Exception safety: nothrow

			TODO: Find a more accurate characterization 
			of the worst-case time. Use that to characterize
			the amortized complexity of monotonic traversal.
			*/
			template <
				bool Right = true, 
				typename DownFilter = All_DownFilter>
			Iterator next(const DownFilter& filter = DownFilter()) const
			{
				Iterator node = *this;
				if (!node.child(Right).isSentinel() &&
					filter.downSet(node.child(Right)))
				{
					return node.child(Right).findFirstBelow(Right, filter);
				}

				// Note that the case when the 'node' is 
				// a sentinel node also gets handled here.
				Iterator prevNode;
				while (true)
				{
					prevNode = node;
					node = node.parent();
					
					if (node.isSentinel())
					{
						break;
					}

					// Note that this is not the same as
					// prevNode == node.child(Right). This
					// is because 'prevNode' can be the 
					// end-node.
					if (prevNode != node.child(!Right))
					{
						continue;
					}

					if (filter.element(node))
					{
						break;
					}

					if (!node.child(Right).isSentinel() &&
						filter.downSet(node.child(Right)))
					{
						node = node.child(Right).findFirstBelow(Right, filter);
						break;
					}
				} 

				return node;
			}

			//! Returns the previous node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			template <typename DownFilter = All_DownFilter>
			Iterator prev(const DownFilter& filter = DownFilter()) const
			{
				return next<false>(filter);
			}

			//! Finds the first element below.
			/*!
			Time complexity: O(?)
			Exception safety: nothrow
			*/
			template <typename DownFilter = All_DownFilter>
			Iterator findFirstBelow(
				bool direction,
				const DownFilter& filter = DownFilter()) const
			{
				auto isBelow = [&](const Iterator& that)
					-> bool
				{
					return !that.isSentinel() &&
						filter.downSet(that);
				};

				ASSERT(isBelow(*this));

				Iterator node = *this;
				while(!node.isSentinel())
				{
					bool right = 
						(isBelow(node.child(!direction)) == !direction);
					if (filter.element(node) && 
						(right == direction))
					{
						break;
					}

					node = node.child(right);
				}

				bool consistentDownFilter = 
					!node.isSentinel();

				// Triggering this is an error on the users side.
				// The provided down-filter was not consistent: it 
				// indicated that there be a marked element in a 
				// subtree, but that was not the case.
				ENSURE(consistentDownFilter);

				return node;
			}

			//! Returns whether the node is red.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool red() const
			{
				return node()->red();
			}

			//! Returns whether the node is black.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool black() const
			{
				return !red();
			}

			//! Returns the number of nodes in the subtree rooted at this node.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			integer size() const
			{
				return node()->size();
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			friend class boost::iterator_core_access;

			explicit Iterator(NodePtr that)
				: Iterator::iterator_adaptor_(that) 
			{
			}

			NodePtr node() const
			{
				return this->base();
			}

			using DereferenceType = std::conditional_t<
				DereferenceToData,
				Data, 
				const Key>;

			DereferenceType& dereference() const
			{
				if constexpr (DereferenceToData) {
					return data();
				} else {
					return key();
				}
			}

			void increment() 
			{ 
				this->base_reference() = next<true>(All_DownFilter()).base();
			}

			void decrement() 
			{ 
				this->base_reference() = next<false>(All_DownFilter()).base();
			}
		};
	
	}

}

#endif
