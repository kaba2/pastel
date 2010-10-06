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

		class Color
		{
		public:
			enum Enum
			{
				Red,
				Black
			};
		};

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

			Color::Enum color() const
			{
				return color_;
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
				Color::Enum color)
				: key_(key)
				, parent_(parent)
				, left_(left)
				, right_(right)
				, color_(color)
			{
			}

			void setColor(Color::Enum color)
			{
				color_ = color;
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
				if (color_ == Color::Red)
				{
					color_ = Color::Black;
				}
				else
				{
					color_ = Color::Red;
				}
			}

			Key key_;
			Node* parent_;
			Node* left_;
			Node* right_;
			Color::Enum color_;
		};

	}

}

#endif
