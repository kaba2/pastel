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

		template <typename>
		class Node;

		//! Base node
		template <typename Settings>
		class Base_Node
		{
		public:
			using Node = Pastel::RedBlackTree_::Node<Settings>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

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

			bool validPropagation() const
			{
				return size_ >= 0;
			}

			integer size() const
			{
				return size_;
			}

		protected:
			Base_Node()
				: parent_(nullptr)
				, child_()
				, red_(false)
				, size_(0)
			{
				isolateSelf();

				/*
				These settings correspond to those of sentinel
				nodes. This is so that RedBlackTree's constructor
				does not have to initialize the sentinel nodes
				to refer to themselves.
				*/
			}

			Base_Node(const Base_Node& that) = delete;
			Base_Node(Base_Node&& that) = delete;
			Base_Node& operator=(Base_Node that) = delete;

			void isolate()
			{
				parent_ = nullptr;
				child_[0] = nullptr;
				child_[1] = nullptr;
				size_ = 0;
				red_ = true;
			}

			void isolateSelf()
			{
				parent_ = (Node*)this;
				child_[0] = parent_;
				child_[1] = parent_;
				size_ = 0;
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

			void setSize(integer size)
			{
				ASSERT_OP(size, >=, 0);
				size_ = size;
			}

			Node*& parent() const
			{
				return (Node*&)parent_;
			}

			Node*& child(bool right) const
			{
				return (Node*&)child_[right];
			}

			Node*& left() const
			{
				return (Node*&)child_[0];
			}

			Node*& right() const
			{
				return (Node*&)child_[1];
			}

			void invalidatePropagation()
			{
				size_ = -1;
			}

			//! The parent node.
			Node* parent_;
			
			//! The child nodes.
			/*
			The child_[0] is the left child, while
			child_[1] is the right child. The array 
			representation is good because it can be indexed 
			with a boolean, and that the boolean can be 
			negated to get to the sibling.

			Visual Studio 2013 has a bug in that it can't
			member-initialize arrays.
			*/
			Node* child_[2];
			
			//! Whether the node is red.
			/*
			A bit-field can not be member-initialized in C++11.
			This is probably a bug in the C++11 standard.
			*/
			uint8 red_ : 1;

			//! The number of elements in the subtree.
			/*!
			The subtree refers to the subtree rooted at 
			this node. This number is needed when splitting
			a red-black tree; otherwise the size of the
			resulting trees can not be computed efficiently.
			*/
			integer size_;
		};

		//! Propagation node
		/*!
		A long-standing bug in Visual Studio 2013 is that
		the empty base-class optimization only works for one 
		empty base-class when using multiple inheritance. 
		Fortunately, this is the case here.
		*/
		template <typename Settings>
		class Propagation_Node
			: public Base_Node<Settings>
			, public Settings::Propagation_Class
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Propagation_Class);

			using Base = Base_Node<Settings>;

			Propagation_Node()
			: Base()
			, Propagation_Class()
			{
			}

			explicit Propagation_Node(const Propagation_Class& propagation)
			: Base()
			, Propagation_Class(propagation)
			{
			}

			const Propagation_Class& propagation() const
			{
				return *this;
			}

		private:
			Propagation_Node(const Propagation_Node& that) = delete;
			Propagation_Node(Propagation_Node&& that) = delete;
			Propagation_Node& operator=(Propagation_Node that) = delete;
		};

		//! Sentinel node
		template <typename Settings>
		class Sentinel_Node
			: public Propagation_Node<Settings>
			, public Settings::SentinelData_Class
		{
		public:
			using Fwd = Settings;

			PASTEL_FWD(Propagation_Class);
			PASTEL_FWD(SentinelData_Class);

			using Base = Propagation_Node<Settings>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, bool>
			friend class Iterator;

			Sentinel_Node()
			: Base()
			, SentinelData_Class()
			{
			}

			explicit Sentinel_Node(
				const Propagation_Class& propagation)
			: Base(propagation)
			, SentinelData_Class()
			{
			}

			SentinelData_Class& sentinelData() const
			{
				return (SentinelData_Class&)*this;
			}

		private:		
			Sentinel_Node(const Sentinel_Node& that) = delete;
			Sentinel_Node(Sentinel_Node&& that) = delete;
			Sentinel_Node& operator=(Sentinel_Node that) = delete;
		};

		//! Key node
		template <typename Settings>
		class Node
			: public Propagation_Node<Settings>
			, public Settings::Data_Class
		{
		public:
			using Fwd = Settings;

			PASTEL_FWD(Key);
			PASTEL_FWD(Propagation_Class);
			PASTEL_FWD(Data_Class);

			using Base = Propagation_Node<Settings>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, bool>
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
			explicit Node(
				const Key& key,
				const Data_Class& data,
				const Propagation_Class& propagation)
				: Base(propagation)
				, Data_Class(data)
				, key_(key)
			{
			}

			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

			Key key_;
		};

	}

}

#endif
