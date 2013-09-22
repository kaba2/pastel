#ifndef PASTELSYS_REDBLACKTREE_NODE_H
#define PASTELSYS_REDBLACKTREE_NODE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/object_forwarding.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	class RedBlackTree;

	namespace RedBlackTree_
	{

		template <typename Key, typename Data_Class_>
		class Node
			: public Data_Class_
		{
		public:
			// We need this to get around a bug in the 
			// Visual Studio 2013 RC compiler.
			using Data_Class = Data_Class_;

			template <typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, bool>
			friend class Iterator;

			enum
			{
				Left = 0,
				Right = 1
			};

			const Key& key() const
			{
				return key_;
			}

			Data_Class& data()
			{
				return *this;
			}

			const Data_Class& data() const
			{
				return *this;
			}

			bool red() const
			{
				return red_;
			}

			bool black() const
			{
				return !red_;
			}
			
			bool isSentinel() const
			{
				// A sentinel is identified by the unique property
				// that its children point to itself.
				return left() == this;
			}

		private:
			Node() = delete;
			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

			Node(Key key,
				Data_Class data,
				Node* parent,
				Node* left,
				Node* right,
				bool red)
				: Data_Class_(std::move(data))
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
				ASSERT(!isSentinel());
				red_ = true;
			}

			void setBlack()
			{
				ASSERT(!isSentinel());
				red_ = false;
			}

			void setRed(bool red)
			{
				ASSERT(!isSentinel());
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
				return child_[Left];
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
				ASSERT(!isSentinel());
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
