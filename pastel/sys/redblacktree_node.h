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
					!boost::is_same<Value, EmptyClass>::value
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
				// that its right child is the sentinel itself.
				return left_ == this;
			}

		private:
			template <typename Key, typename Compare, typename RbtPolicy>
			friend class RedBlackTree;

			template <typename Key, typename Value>
			friend class Iterator;

			Node(const Key& key,
				Node* parent,
				Node* left,
				Node* right,
				bool red)
				: key_(key)
				, parent_(parent)
				, left_(left)
				, right_(right)
				, red_(red)
			{
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

			Node*& left()
			{
				return left_;
			}

			Node* left() const
			{
				return left_;
			}

			Node*& right()
			{
				return right_;
			}

			Node* right() const
			{
				return right_;
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
			Node* left_;
			Node* right_;
			bool red_;
		};

	}

}

#endif
