#ifndef PASTEL_REDBLACKTREE_NODE_H
#define PASTEL_REDBLACKTREE_NODE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/possiblyemptymember.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	class RedBlackTree;

	namespace RedBlackTree_Detail
	{

		template <typename Key, typename Value>
		class ConstIterator;

		template <typename Key, typename Value>
		class Node
			: private PossiblyEmptyMember<Value>
		{
		public:
			typedef PossiblyEmptyMember<Value> Base;

			enum
			{
				ValueExists = 
					!std::is_same<Value, EmptyClass>::value
			};

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
			
			Value& value()
			{
				PASTEL_STATIC_ASSERT(ValueExists);

				return *Base::data();
			}

			const Value& value() const
			{
				PASTEL_STATIC_ASSERT(ValueExists);

				return *Base::data();
			}

			bool sentinel() const
			{
				// A sentinel is identified by the unique property
				// that its children point to itself.
				return left() == this;
			}

		private:
			template <typename, typename, typename>
			friend class RedBlackTree;

			template <typename, typename>
			friend class Iterator;

			Node(const Key& key,
				Node* parent,
				Node* left,
				Node* right,
				bool red)
				: key_(key)
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

			Value* valuePtr() const
			{
				return (Value*)Base::data();
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
