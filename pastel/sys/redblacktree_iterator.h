// Description: Red-black tree iterator

#ifndef PASTELSYS_REDBLACKTREE_ITERATOR_H
#define PASTELSYS_REDBLACKTREE_ITERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblacktree_node.h"

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
			typename Node_Settings::Data_Class, 
			const typename Node_Settings::Key>::type,
			boost::bidirectional_traversal_tag>
		{
		public:
			using Fwd = Node_Settings;

			PASTEL_FWD(Key);
			PASTEL_FWD(Data_Class);
			PASTEL_FWD(Propagation_Class);

			Iterator()
				: Iterator::iterator_adaptor_(0) 
			{
			}

			Iterator(NodePtr that)
				: Iterator::iterator_adaptor_(that) 
			{
			}

			template <
				typename That_NodePtr,
				bool That_DereferenceToData,
				typename = PASTEL_ENABLE_IF((boost::is_convertible<That_NodePtr, NodePtr>), void)>
			Iterator(
				const Iterator<That_NodePtr, Node_Settings, That_DereferenceToData>& that)
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			const Key& key() const
			{
				PENSURE(!isSentinel());
				return node()->key();
			}

			const Propagation_Class& propagation() const
			{
				return node()->propagation();
			}

			Data_Class& data() const
			{
				PENSURE(!isSentinel());
				return (Data_Class&)node()->data();
			}

			bool isSentinel() const
			{
				return node()->isSentinel();
			}

			bool parentExists() const
			{
				return node()->parentExists();
			}

			bool childExists(bool right) const
			{
				return node()->childExists(right);
			}

			Iterator parent() const
			{
				return Iterator(node()->parent());
			}

			Iterator left() const
			{
				return Iterator(node()->left());
			}

			Iterator right() const
			{
				return Iterator(node()->right());
			}

			Iterator child(bool right) const
			{
				return Iterator(node()->child(right));
			}

			bool red() const
			{
				return node()->red();
			}

			bool black() const
			{
				return !red();
			}

			integer size() const
			{
				return node()->size();
			}

		private:
			friend class boost::iterator_core_access;

			using DereferenceType = typename std::conditional<
				DereferenceToData,
				Data_Class, const Key>::type;

			struct KeyTag {};
			struct DataTag {};

			using DereferenceTag = typename std::conditional<
				DereferenceToData, 
				DataTag, KeyTag>::type;

			NodePtr node() const
			{
				return this->base();
			}

			const Key& dereference(KeyTag) const
			{
				return key();
			}

			Data_Class& dereference(DataTag) const
			{
				return data();
			}

			DereferenceType& dereference() const
			{
				return dereference(DereferenceTag());
			}

			template <bool Right>
			void increment_()
			{
				NodePtr& node_ = this->base_reference();

				if (node_->child(Right)->isSentinel())
				{
					NodePtr prevNode = node_;
					do
					{
						prevNode = node_;
						node_ = node_->parent();
						// It is essential to test for 
						// prevNode != node_->child(!Right)
						// rather than 
						// prevNode == node_->child(Right).
						// The difference occurs when 'node'
						// is the end-node.
					} while (!node_->isSentinel() &&
						prevNode != node_->child(!Right));
				}
				else
				{
					node_ = node_->child(Right);
					while (!node_->child(!Right)->isSentinel())
					{
						node_ = node_->child(!Right);
					}
				}
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

#endif
