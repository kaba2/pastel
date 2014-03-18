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

		template <typename Node>
		class Node_Base
		{
		public:
			template <typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, bool>
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
			Node_Base(const Node_Base& that) = delete;
			Node_Base(Node_Base&& that) = delete;
			Node_Base& operator=(Node_Base that) = delete;

			explicit Node_Base(Node_Base* sentinel)
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
				parent_ = (Node*)sentinel;
				child_[Left] = (Node*)sentinel;
				child_[Right] = (Node*)sentinel;
			}

			void setRed()
			{
				ASSERT(!isSentinel());
				red_ = true;
			}

			void setBlack()
			{
				red_ = false;
			}

			void setRed(bool red)
			{
				ASSERT(!isSentinel() || !red);
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
		class Node
			: public Node_Base<Node<Key, Data_Class_>>
			, public Data_Class_
		{
		public:
			using Base = Node_Base<Node<Key, Data_Class_>>;

			// We need this to get around a bug in the 
			// Visual Studio 2013 RC compiler.
			using Data_Class = Data_Class_;

			template <typename, typename>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, bool>
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
			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

			template <typename... Value>
			Node(
				Base* sentinel,
				Key&& key,
				Value&&... value)
				: Base(sentinel)
				, Data_Class_(std::forward<Value>(value)...)
				, key_(std::move(key))
			{
			}

			Key key_;
		};

	}

}

#endif
