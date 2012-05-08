#ifndef PASTEL_TREE_NODE_H
#define PASTEL_TREE_NODE_H

#include "pastel/sys/tree.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	namespace Tree_
	{

		class Sentinel_Node;

		class Node
		{
		public:
			Node();
			explicit Node(Sentinel_Node* sentinel);
			~Node();

			bool empty() const;
			Node* child(integer childIndex) const;

			void setChild(
				integer childIndex,
				Node* child);

			Node* parent;

		private:
			Tuple<Node*, 2> childSet;
		};

		class Sentinel_Node
			: public Node
		{
		public:
			Sentinel_Node();

			integer count() const;

			void increaseCount(integer amount = 1);
			void decreaseCount(integer amount = 1);

		private:
			integer count_;
		};

		Node::Node()
			: parent(0)
			, childSet(0)
		{
			// Can't use 'this' in the initializer list,
			// because the object is not yet constructed.
			parent = this;
			childSet.set(this);
		}

		Node::Node(Sentinel_Node* sentinel)
			: parent(sentinel)
			, childSet(0)
		{
			// We will set the sentinel references here
			// because the construction of the Tuple
			// might fail.
			childSet.set(sentinel);
			sentinel->increaseCount(2);
		}

		Node::~Node()
		{
			if (!empty())
			{
				for (integer i = 0;i < 2;++i)
				{
					Node*& child = childSet[i];
					ASSERT(child);
					if (child->empty())
					{
						Sentinel_Node* sentinelNode =
							(Sentinel_Node*)child;
						sentinelNode->decreaseCount();
						child = 0;
					}
				}
			}
		}

		bool Node::empty() const
		{
			// A sentinel is identified by its
			// children pointing to itself.
			return childSet[0] == this;
		}

		Node* Node::child(integer childIndex) const
		{
			return childSet[childIndex];
		}

		void Node::setChild(
			integer childIndex,
			Node* child)
		{
			ASSERT(child);
			ASSERT(!empty());
			ASSERT_OP(childIndex, >=, 0);
			ASSERT_OP(childIndex, <, 2);

			if (child->empty())
			{
				Sentinel_Node* newSentinel =
					(Sentinel_Node*)child;
				newSentinel->increaseCount();
			}

			Node*& childRef = childSet[childIndex];
			if (childRef->empty())
			{
				Sentinel_Node* oldSentinel =
					(Sentinel_Node*)childRef;
				oldSentinel->decreaseCount();
			}

			childRef = child;
		}

		Sentinel_Node::Sentinel_Node()
			: Node()
			, count_(0)
		{
		}

		integer Sentinel_Node::count() const
		{
			return count_;
		}

		void Sentinel_Node::increaseCount(integer amount)
		{
			ASSERT_OP(amount, >, 0);
			count_ += amount;
		}

		void Sentinel_Node::decreaseCount(integer amount)
		{
			ASSERT_OP(amount, >, 0);
			ASSERT_OP(count_, >=, amount);

			count_ -= amount;
			if (count_ == 0)
			{
				delete this;
			}
		}

		template <typename Type>
		class Data_Node
			: public Node
		{
		public:
			explicit Data_Node(Sentinel_Node* sentinel)
				: Node(sentinel)
				, data_()
			{
			}

			Type* data()
			{
				return (Type*)data_;
			}

			const Type* data() const
			{
				return (Type*)data_;
			}

		private:
			char data_[sizeof(Type)];
		};

		template <>
		class Data_Node<EmptyClass>
			: public Node
		{
		public:
			Data_Node()
				: Node()
			{
			}

			explicit Data_Node(Sentinel_Node* sentinel)
				: Node(sentinel)
			{
			}

			EmptyClass* data()
			{
				return 0;
			}

			const EmptyClass* data() const
			{
				return 0;
			}
		};

	}

}

#endif
