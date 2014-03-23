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

		//! The base node.
		template <typename Settings>
		class Node_Base
		{
		public:
			using Node = Pastel::RedBlackTree_::Node<Settings>;

			template <typename, template <typename> class>
			friend class Pastel::RedBlackTree;

			template <typename, typename, bool>
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
			Node_Base()
				: parent_(0)
				, child_()
				, red_(true)
			{
			}

			Node_Base(const Node_Base& that) = delete;
			Node_Base(Node_Base&& that) = delete;
			Node_Base& operator=(Node_Base that) = delete;

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
		template <typename Settings>
		class Data_Node
			: public Node_Base<Settings>
			, public Settings::Propagation_Class
		{
		public:
			using Fwd = Settings;
			PASTEL_FWD(Propagation_Class);

			using Base = Node_Base<Settings>;

			Data_Node()
			: Base()
			, Propagation_Class()
			{
			}

			explicit Data_Node(const Propagation_Class& propagation)
			: Base()
			, Propagation_Class(propagation)
			{
			}

			const Propagation_Class& propagation() const
			{
				return *this;
			}

		private:
			Data_Node(const Data_Node& that) = delete;
			Data_Node(Data_Node&& that) = delete;
			Data_Node& operator=(Data_Node that) = delete;
		};

		//! Key node
		template <typename Settings>
		class Node
			: public Data_Node<Settings>
			, public Settings::Data_Class
		{
		public:
			using Fwd = Settings;

			PASTEL_FWD(Key);
			PASTEL_FWD(Propagation_Class);
			PASTEL_FWD(Data_Class);
			PASTEL_CONSTEXPR bool UseSentinelData = 
				Settings::UseSentinelData;

			using Base = Data_Node<Settings>;

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
				PENSURE(!isSentinel() || UseSentinelData);
				return *this;
			}

			const Data_Class& data() const
			{
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
