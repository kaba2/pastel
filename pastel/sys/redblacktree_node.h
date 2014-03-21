// Description: Red-black tree node

#ifndef PASTELSYS_REDBLACKTREE_NODE_H
#define PASTELSYS_REDBLACKTREE_NODE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/object_forwarding.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	class RedBlackTree;

	namespace RedBlackTree_
	{

		template <typename, typename>
		class Node;

		//! The base of all nodes.
		/*!
		There are two kinds of nodes: sentinels and data nodes.
		The difference between the two is that the sentinels do
		not contain a key. However, the sentinels do contain
		user data. This is important for usability because it 
		reduces complexity when updating hierarchical information. 
		Without data in sentinel nodes one has to check, in the
		update procedure updateHierarchical(), for a sentinel, 
		rather than relying on the default sentinel data.
		*/
		template <typename Key, typename Data_Class_>
		class Node_Base
			: public Data_Class_
		{
		public:
			// We need this to get around a bug in the 
			// Visual Studio 2013 Update 1 compiler.
			using Data_Class = Data_Class_;

			using Node = Pastel::RedBlackTree_::Node<Key, Data_Class>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, bool>
			friend class Iterator;

			bool red() const
			{
				return red_;
			}

			bool black() const
			{
				return !red();
			}
			
			bool isSentinel() const
			{
				// A sentinel is identified by the unique property
				// that its left child points to itself.
				return left() == this;
			}

			Data_Class& data()
			{
				return *this;
			}

			const Data_Class& data() const
			{
				return *this;
			}

		protected:
			Node_Base(const Node_Base& that) = delete;
			Node_Base(Node_Base&& that) = delete;
			Node_Base& operator=(Node_Base that) = delete;

			template <typename... Value>
			explicit Node_Base(
				Value&&... value)
				: Data_Class(std::forward<Value>(value)...)
				, parent_(0)
				, child_()
				, red_(true)
			{
			}

			void isolate()
			{
				parent_ = 0;
				child_[0] = 0;
				child_[1] = 0;
			}

			void setRed()
			{
				setRed(true);
			}

			void setBlack()
			{
				setRed(false);
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

			Node*& child(bool right)
			{
				return child_[right];
			}

			Node* child(bool right) const
			{
				return child_[right];
			}

			Node*& left()
			{
				return child_[0];
			}

			Node* left() const
			{
				return child_[0];
			}

			Node*& right()
			{
				return child_[1];
			}

			Node* right() const
			{
				return child_[1];
			}

			Node* parent_;
			Node* child_[2];
			uint8 red_ : 1;
		};

		//! Data node
		template <typename Key, typename Data_Class_>
		class Node
			: public Node_Base<Key, Data_Class_>
		{
		public:
			using Base = Node_Base<Key, Data_Class_>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, typename, bool>
			friend class Iterator;

			const Key& key() const
			{
				ASSERT(!isSentinel());
				return key_;
			}

		private:
			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

			template <typename... Value>
			Node(
				Key&& key,
				Value&&... value)
				: Base(std::forward<Value>(value)...)
				, key_(std::move(key))
			{
			}

			Key key_;
		};

	}

}

#endif
