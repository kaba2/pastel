// Description: Red-black tree node

#ifndef PASTELSYS_REDBLACKTREE_NODE_H
#define PASTELSYS_REDBLACKTREE_NODE_H

#include "pastel/sys/redblacktree/redblacktree_fwd.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename, template <typename> class>
	class RedBlackTree;

	namespace RedBlackTree_
	{

		//! Base node
		class Node
		{
		public:
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
			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, bool>
			friend class Iterator;

			Node()
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

			Node(const Node& that) = delete;
			Node(Node&& that) = delete;
			Node& operator=(Node that) = delete;

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
		template <typename Settings>
		class Propagation_Node
			: public Node
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Propagation);

			const Propagation& propagation() const
			{
				return propagation_;
			}

		protected:
			Propagation_Node() = default;

			Propagation_Node(const Propagation_Node& that) = delete;
			Propagation_Node(Propagation_Node&& that) = delete;
			Propagation_Node& operator=(Propagation_Node that) = delete;

		private:
			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			Propagation propagation_;
		};

		//! Sentinel node
		template <typename Settings>
		class Sentinel_Node
			: public Settings::EndBase
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(SentinelData);
			PASTEL_FWD(EndBase);

			Sentinel_Node() = default;

			SentinelData& sentinelData()
			{
				return data_;
			}

			const SentinelData& sentinelData() const
			{
				return data_;
			}

		protected:
			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			Sentinel_Node(const Sentinel_Node& that) = delete;
			Sentinel_Node(Sentinel_Node&& that) = delete;
			Sentinel_Node& operator=(Sentinel_Node that) = delete;

			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			SentinelData data_;
		};

		//! Data node
		template <typename Settings>
		class Data_Node
			: public Propagation_Node<Settings>
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Data);

			Data& data()
			{
				return data_;
			}

			const Data& data() const
			{
				return data_;
			}

		protected:
			using Base = Propagation_Node<Settings>;
			PASTEL_FWD(Propagation);

			Data_Node() = default;

			explicit Data_Node(const Data& data)
				: Base()
				, data_(data)
			{
			}

			Data_Node(const Data_Node& that) = delete;
			Data_Node(Data_Node&& that) = delete;
			Data_Node& operator=(Data_Node that) = delete;

		private:
			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			Data data_;
		};

		//! Key node
		template <typename Settings_>
		class Key_Node
			: public Data_Node<Settings_>
		{
		public:
			using Fwd = Settings_;

			PASTEL_FWD(Key);

			const Key& key() const
			{
				return key_;
			}

		protected:
			using Base = Data_Node<Settings_>;
			PASTEL_FWD(Data);

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			Key_Node(
				const Key& key,
				const Data& data)
				: Base(data)
				, key_(key)
			{
			}

			Key_Node() = delete;
			Key_Node(const Key_Node& that) = delete;
			Key_Node(Key_Node&& that) = delete;
			Key_Node& operator=(Key_Node that) = delete;

		private:
			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			Key key_;
		};

	}

}

#endif
