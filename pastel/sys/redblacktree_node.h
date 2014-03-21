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

		template <typename, typename, bool>
		class Node;

		//! The base node.
		template <typename Key, typename Data_Class_, bool UseSentinelData>
		class Node_Base
		{
		public:
			using Node = Pastel::RedBlackTree_::Node<Key, Data_Class_, UseSentinelData>;

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

		protected:
			Node_Base(const Node_Base& that) = delete;
			Node_Base(Node_Base&& that) = delete;
			Node_Base& operator=(Node_Base that) = delete;

			Node_Base()
				: parent_(0)
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
		/*!
		A long-standing bug in Visual Studio 2013 is that
		the empty base-class optimization only works for one 
		empty base-class when using multiple inheritance. 
		Fortunately, this is the case here.
		*/
		template <typename Key, typename Data_Class_, bool UseSentinelData>
		class Data_Node
			: public Data_Class_
			, public Node_Base<Key, Data_Class_, UseSentinelData>
		{
		public:
			// We need this to get around a bug in the 
			// Visual Studio 2013 Update 1 compiler.
			using Data_Class = Data_Class_;

			template <typename... Value>
			explicit Data_Node(
				Value&&... value)
				: Data_Class(std::forward<Value>(value)...)
			{
			}

			Data_Class& data()
			{
				PENSURE(!isSentinel() || UseSentinelData);
				return *this;
			}

			const Data_Class& data() const
			{
				return *this;
			}

		private:
			Data_Node(const Data_Node& that) = delete;
			Data_Node(Data_Node&& that) = delete;
			Data_Node& operator=(Data_Node that) = delete;
		};

		//! Key node
		template <typename Key, typename Data_Class_, bool UseSentinelData>
		class Node
			: public Data_Node<Key, Data_Class_, UseSentinelData>
		{
		public:
			using Base = Data_Node<Key, Data_Class, UseSentinelData>;

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
