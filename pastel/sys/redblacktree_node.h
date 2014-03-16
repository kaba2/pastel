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

		class Node
		{
		public:
			template <typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, typename, bool>
			friend class Iterator;

			enum
			{
				Left = 0,
				Right = 1
			};

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

		protected:
			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

			explicit Node(Node* sentinel)
				: parent_(0)
				, child_()
				, red_(sentinel != 0)
			{
				if (sentinel == 0)
				{
					// A null sentinel means that
					// the node itself is the sentinel.
					sentinel = this;
				}

				// The sentinel node is black.
				// The leaf nodes are red.
				parent_ = sentinel;
				child_[Left] = sentinel;
				child_[Right] = sentinel;
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

			Node* parent_;
			Node* child_[2];
			bool red_;
		};

		/*!
		The empty base-class optimization does not work in
		general in Visual Studio 2013 for multiple empty
		base-classes in multiple inheritance. This is a 
		long-standing bug. Since here we have only one
		potentially empty base-class, this gets optimized 
		correctly.
		*/
		template <typename Key, typename Data_Class_>
		class Data_Node
			: public Node
			, public Data_Class_
		{
		public:
			// We need this to get around a bug in the 
			// Visual Studio 2013 RC compiler.
			using Data_Class = Data_Class_;

			template <typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, typename, bool>
			friend class Iterator;

			const Key& key() const
			{
				ASSERT(!isSentinel());
				return key_;
			}

			Data_Class& data()
			{
				ASSERT(!isSentinel());
				return *this;
			}

			const Data_Class& data() const
			{
				ASSERT(!isSentinel());
				return *this;
			}

		private:
			Data_Node(const Data_Node& that) = delete;
			Data_Node(Data_Node&& that) = delete;
			Data_Node& operator=(Data_Node that) = delete;

			template <typename... Value>
			Data_Node(
				Node* sentinel,
				Key&& key,
				Value&&... value)
				: Node(sentinel)
				, Data_Class_(std::forward<Value>(value)...)
				, key_(std::move(key))
			{
			}

			Key key_;
		};

	}

}

#endif
