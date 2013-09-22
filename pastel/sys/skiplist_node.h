// Description: Skip list node

#ifndef PASTELSYS_SKIPLIST_NODE_H
#define PASTELSYS_SKIPLIST_NODE_H

#include "pastel/sys/skiplist.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/named_tuples.h"

#include <vector>

namespace Pastel
{

	namespace SkipList_
	{

		class Node;

		class SuperNode
		{
		public:
			explicit SuperNode(Node* repr)
			: repr_(repr)
			{
			}

			Node*& repr()
			{
				return repr_;
			}

			Node* repr() const
			{
				return repr_;
			}

			Node* repr_;
		};

		class Node
		{
		public:
			explicit Node(
				integer levels,
				SuperNode* super)
			: link_(levels)
			, super_(super)
			{
				ASSERT_OP(link_.size(), >= , 2);
			}

			template <bool Direction>
			Node*& link(integer i)
			{
                ASSERT_OP(i, >=, 0);
                ASSERT_OP(i, <, link_.size());
				return link_[i].next[Direction];
			}

			Node*& link(integer i, bool Direction)
			{
                ASSERT_OP(i, >=, 0);
                ASSERT_OP(i, <, link_.size());
				return link_[i].next[Direction];
			}

			template <bool Direction>
			Node* link(integer i) const
			{
				return link_[i].next[Direction];
			}

			Node* link(integer i, bool Direction) const
			{
				return link_[i].next[Direction];
			}

			integer size() const
			{
				return link_.size();
			}

			SuperNode*& super()
			{
				return super_;
			}

			SuperNode* super() const
			{
				return super_;
			}

			Node* repr() const
			{
				return super()->repr();
			}

			bool isRepresentative() const
			{
				return link<true>(1) != 0;
			}

		//private:
			struct Link
			{
				Node* next[2];
			};
			
			std::vector<Link> link_;
			SuperNode* super_;
		};

		template <
			typename Key, 
			typename Value_Class>
		class Data_Node
		: public Node
		, public Value_Class
		{
		public:
			Data_Node(
				integer levels, 
				SuperNode* super,
				Key key,
				Value_Class data)
			: Node(levels, super)
			, Value_Class(std::move(data))
			, key_(std::move(key))
			{
			}

			const Key& key() const
			{
				return key_;
			}

			Value_Class& value()
			{
				return *this;
			}

			const Value_Class& value() const
			{
				return *this;
			}

		private:
			Key key_;
		};

	}

}

#endif
