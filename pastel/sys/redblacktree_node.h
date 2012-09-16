#ifndef PASTELSYS_REDBLACKTREE_NODE_H
#define PASTELSYS_REDBLACKTREE_NODE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/object_forwarding.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename Data, typename Customization>
	class RedBlackTree;

	namespace RedBlackTree_
	{

		template <typename Key, typename Data>
		class ConstIterator;

		template <typename Key, typename Data>
		class Node
			: public AsClass<Data>::type
		{
		public:
			template <typename, typename, typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename>
			friend class Iterator;

			typedef typename AsClass<Data>::type
				Data_Class;

			enum
			{
				Left = 0,
				Right = 1
			};

			const Key& key() const
			{
				return key_;
			}

			bool red() const
			{
				return red_;
			}

			bool black() const
			{
				return !red_;
			}
			
			bool sentinel() const
			{
				// A sentinel is identified by the unique property
				// that its children point to itself.
				return left() == this;
			}

		private:
			Node() PASTEL_DELETE;
			Node(const Node& that) PASTEL_DELETE;
			Node(Node&& that) PASTEL_DELETE;
			Node& operator=(Node that) PASTEL_DELETE;

			Node(Key key,
				Data_Class data,
				Node* parent,
				Node* left,
				Node* right,
				bool red)
				: Data_Class(std::move(data))
				, key_(std::move(key))
				, parent_(parent)
				, child_()
				, red_(red)
			{
				child_[Left] = left;
				child_[Right] = right;
			}

			void setRed()
			{
				ASSERT(!sentinel());
				red_ = true;
			}

			void setBlack()
			{
				ASSERT(!sentinel());
				red_ = false;
			}

			void setRed(bool red)
			{
				ASSERT(!sentinel());
				red_ = red;
			}

			Node*& parent()
			{
				return parent_;
			}

			Node* parent() const
			{
				return parent_;
			}

			Node*& child(integer direction)
			{
				ASSERT(direction == Left || 
					direction == Right);
				return child_[direction];
			}

			Node* child(integer direction) const
			{
				ASSERT(direction == Left || 
					direction == Right);
				return child_[direction];
			}

			Node*& left()
			{
				return child_[Left];
			}

			Node* left() const
			{
				return child_[Right];
			}

			Node*& right()
			{
				return child_[Right];
			}

			Node* right() const
			{
				return child_[Right];
			}

			void flipColor()
			{
				ASSERT(!sentinel());
				red_ = !red_;
			}

			Key key_;
			Node* parent_;
			Node* child_[2];
			bool red_;
		};

	}

}

#endif
