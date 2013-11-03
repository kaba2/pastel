// Description: Skip list node

#ifndef PASTELSYS_SKIPLIST_NODE_H
#define PASTELSYS_SKIPLIST_NODE_H

#include "pastel/sys/skiplist.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/named_tuples.h"

#include <vector>
#include <memory>

namespace Pastel
{

	namespace SkipList_
	{

		class Node;

		class Link
		{
		public:
			Node*& operator[](bool direction)
			{
				return link_[direction];
			}

			Node* const operator[](bool direction) const
			{
				return link_[direction];
			}

		private:
			Node* link_[2];
		};

		using LinkSet = std::unique_ptr<Link[]>;

		class SuperNode
		{
		public:
			SuperNode() = delete;
			SuperNode(const SuperNode&) = delete;
			SuperNode(SuperNode&&) = delete;

			explicit SuperNode(Node* repr)
			: repr_(repr)
			, keys_(1)
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

			integer& keys()
			{
				return keys_;
			}

			integer keys() const
			{
				return keys_;
			}

			Node* repr_;
			integer keys_;
		};

		class Node
		{
		public:
			Node() 
			: linkSet_()
			, levels_(0)
			, super_(0)
			{
			}

			Node(const Node&) = delete;
			Node(Node&&) = delete;

			~Node()
			{
				clear();
			}

			void clear()
			{
				linkSet_.reset();
				levels_ = 0;
			}

			Link& link(integer i)
			{
                ASSERT_OP(i, >=, 0);
                ASSERT_OP(i, <, levels_);
				return linkSet_[i];
			}

			const Link& link(integer i) const
			{
                ASSERT_OP(i, >=, 0);
                ASSERT_OP(i, <, levels_);
				return linkSet_[i];
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
				// The equivalence class is stored
				// if and only if there is more than
				// one element in the equivalence class.
				if (super())
				{
					return super()->repr();
				}

				return (Node*)this;
			}

			integer keys() const
			{
				if (super())
				{
					return super()->keys();
				}
				
				return 1;
			}

			void setLinkSet(LinkSet&& linkSet, integer levels)
			{
				linkSet_ = std::move(linkSet);
				levels_ = levels;
			}

			bool isRepresentative() const
			{
				return repr() == this;
			}

			integer levels() const
			{
				return levels_;
			}

		//private:
			LinkSet linkSet_;
			integer levels_;
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
			Data_Node() = delete;
			Data_Node(const Data_Node&) = delete;
			Data_Node(Data_Node&&) = delete;

			Data_Node(
				Key key,
				Value_Class data)
			: Node()
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
