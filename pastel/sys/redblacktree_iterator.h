#ifndef PASTEL_REDBLACKTREE_ITERATOR_H
#define PASTEL_REDBLACKTREE_ITERATOR_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblacktree_node.h"

#include <boost/iterator.hpp>

namespace Pastel
{

	namespace RedBlackTree_Detail
	{

		template <typename Key, typename Value>
		class Iterator;

		template <typename Key, typename Value>
		class ConstIterator
			: public boost::bidirectional_iterator_helper<
			ConstIterator<Key, Value>, const Node<Key, Value>, integer>
		{
		public:
			template <typename Key, typename Value>
			friend class Iterator;

			typedef Node<Key, Value> Node;

			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			ConstIterator()
				: node_(0)
			{
			}

			ConstIterator(const ConstIterator& that)
				: node_(that.node_)
			{
			}

			bool operator==(const ConstIterator& that) const
			{
				return node_ == that.node_;
			}

			bool operator<(const ConstIterator& that) const
			{
				return node_ < that.node_;
			}
			
			ConstIterator& operator++()
			{
				if (node_->right()->sentinel())
				{
					const Node* prevNode = node_;
					do
					{
						prevNode = node_;
						node_ = node_->parent();
					}
					while (prevNode == node_->right());
				}
				else
				{
					node_ = node_->right();
					while(!node_->left()->sentinel())
					{
						node_ = node_->left();
					}
				}

				return *this;
			}

			ConstIterator& operator--()
			{
				if (node_->left()->sentinel())
				{
					const Node* originalNode = node_;
					const Node* prevNode = node_;
					do
					{
						prevNode = node_;
						node_ = node_->parent();
					}
					while (prevNode == node_->left());
					
					if (node_->sentinel())
					{
						node_ = originalNode;
					}
				}
				else
				{
					node_ = node_->left();
					while(!node_->right()->sentinel())
					{
						node_ = node_->right();
					}
				}

				return *this;
			}

			const Node& operator*() const
			{
				ASSERT(node_);
				return *node_;
			}

			bool sentinel() const
			{
				return node_->sentinel();
			}

			ConstIterator parent() const
			{
				return ConstIterator(node_->parent());
			}

			ConstIterator left() const
			{
				return ConstIterator(node_->left());
			}

			ConstIterator right() const
			{
				return ConstIterator(node_->right());
			}

		private:
			template <typename Key, typename Compare, typename RbtPolicy>
			friend class RedBlackTree;

			explicit ConstIterator(const Node* node)
				: node_(node)
			{
			}

			const Node* node_;
		};

		template <typename Key, typename Value>
		class Iterator
			: public ConstIterator<Key, Value>
		{
		public:
			typedef ConstIterator<Key, Value> Base;

			using Base::Node;
			
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			Iterator()
				: node_(0)
			{
			}

			Iterator& operator++()
			{
				return (Iterator&)Base::operator++();
			}

			Iterator& operator--()
			{
				return (Iterator&)Base::operator--();
			}

			Node& operator*()
			{
				ASSERT(node_);
				return (Node&)*node_;
			}

			Iterator parent() const
			{
				return Iterator(node_->parent());
			}

			Iterator left() const
			{
				return Iterator(node_->left());
			}

			Iterator right() const
			{
				return Iterator(node_->right());
			}

		private:
			template <typename Key, typename Compare, typename RbtPolicy>
			friend class RedBlackTree;

			explicit Iterator(Node* node)
				: Base(node)
			{
			}
		};

	}

}

#endif
