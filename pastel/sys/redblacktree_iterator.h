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
		private:
			struct enabler {};

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
				bool That_DereferenceToData>
			Iterator(
				const Iterator<That_NodePtr, Node_Settings, That_DereferenceToData>& that,
				typename boost::enable_if<
				boost::is_convertible<That_NodePtr, NodePtr>, 
				enabler>::type = enabler())
				: Iterator::iterator_adaptor_(that.base()) 
			{
			}

			const Key& key() const
			{
				return node()->key();
			}

			const Propagation_Class& propagation() const
			{
				return node()->propagation();
			}

			Data_Class& data() const
			{
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

			void increment() 
			{ 
				NodePtr& node_ = this->base_reference();

				if (node_->right()->isSentinel())
				{
					NodePtr prevNode = node_;
					do
					{
						prevNode = node_;
						node_ = node_->parent();
					}
					while (!node_->isSentinel() && 
						prevNode == node_->right());
				}
				else
				{
					node_ = node_->right();
					while(!node_->left()->isSentinel())
					{
						node_ = node_->left();
					}
				}
			}

			void decrement() 
			{ 
				NodePtr& node_ = this->base_reference();

				if (node_->left()->isSentinel())
				{
					NodePtr originalNode = node_;
					NodePtr prevNode = node_;
					do
					{
						prevNode = node_;
						node_ = node_->parent();

						// In case 'prevNode' is the sentinel
						// node, it matters whether we test
						// prevNode == node->left() or 
						// prevNode != node->right(). The
						// latter is the correct test.
					}
					while (!node_->isSentinel() && 
						prevNode != node_->right());
				}
				else
				{
					node_ = node_->left();
					while(!node_->right()->isSentinel())
					{
						node_ = node_->right();
					}
				}
			}
		};
	
	}

}

#endif
