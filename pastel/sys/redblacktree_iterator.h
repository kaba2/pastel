#ifndef PASTEL_REDBLACKTREE_ITERATOR_H
#define PASTEL_REDBLACKTREE_ITERATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblacktree_node.h"

#include <boost/operators.hpp>
#include <boost/iterator.hpp>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename Key, typename Data>
		class ConstIterator;

		template <typename Key, typename Data>
		class Iterator
			: public boost::bidirectional_iterator_helper<
			Iterator<Key, Data>, Key, integer>
		{
		public:
			template <typename, typename>
			friend class ConstIterator;

			typedef RedBlackTree_::Node<Key, Data> Node;

			typedef typename AsClass<Data>::type Data_Class;

			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			Iterator()
				: node_(0)
			{
			}

			bool operator==(const Iterator& that) const
			{
				return node_ == that.node_;
			}

			bool operator<(const Iterator& that) const
			{
				return node_ < that.node_;
			}
			
			Iterator& operator++()
			{
				if (node_->right()->sentinel())
				{
					Node* prevNode = node_;
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

			Iterator& operator--()
			{
				if (node_->left()->sentinel())
				{
					Node* originalNode = node_;
					Node* prevNode = node_;
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
					while (!node_->sentinel() && 
						prevNode != node_->right());
					
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

			const Key& operator*() const
			{
				return node_->key();
			}

			const Key& key() const
			{
				return node_->key();
			}

			Data_Class& data() const
			{
				return *node_;
			}

			bool sentinel() const
			{
				return node_->sentinel();
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

			bool red() const
			{
				return node_->red();
			}

			bool black() const
			{
				return !red();
			}

		private:
			template <typename, typename, typename, typename>
			friend class RedBlackTree;

			explicit Iterator(Node* node)
				: node_(node)
			{
			}

			Node* node_;
		};

		template <typename Key, typename Data>
		class ConstIterator
			: public boost::bidirectional_iterator_helper<
			ConstIterator<Key, Data>, const Key, integer>
		{
		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			typedef RedBlackTree_::Iterator<Key, Data> Iterator;
			typedef RedBlackTree_::Node<Key, Data> Node;
			
			typedef typename AsClass<Data>::type Data_Class;

			ConstIterator()
				: iter_()
			{
			}

			ConstIterator(const Iterator& that)
				: iter_(that)
			{
			}

			bool operator==(const ConstIterator& that) const
			{
				return iter_ == that.iter_;
			}

			bool operator<(const ConstIterator& that) const
			{
				return iter_ < that.iter_;
			}
			
			ConstIterator& operator++()
			{
				++iter_;
				return *this;
			}

			ConstIterator& operator--()
			{
				--iter_;
				return *this;
			}

			const Key& operator*() const
			{
				return *iter_;
			}

			const Key& key() const
			{
				return iter_.key();
			}

			const Data_Class& data() const
			{
				return iter_.data();
			}

			bool sentinel() const
			{
				return iter_.sentinel();
			}

			ConstIterator parent() const
			{
				return iter_.parent();
			}

			ConstIterator left() const
			{
				return iter_.left();
			}

			ConstIterator right() const
			{
				return iter_.right();
			}

			bool red() const
			{
				return iter_.red();
			}

			bool black() const
			{
				return !red();
			}

		private:
			template <typename, typename, typename, typename>
			friend class RedBlackTree;

			explicit ConstIterator(const Node* node)
				: iter_((Node*)node)
			{
			}

			Iterator iter_;
		};

	}

}

#endif
