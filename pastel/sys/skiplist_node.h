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
			explicit Node(integer levels = 0) 
			: link_(levels)
			, super_(0)
			{
			}

			Node(const Node&) = delete;
			Node(Node&&) = delete;
		
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

			bool isRepresentative() const
			{
				return repr() == this;
			}

			void addLevel()
			{
				integer capacity = link_.capacity();
				if (levels() == capacity)
				{
					// The addition of a new level causes
					// a reallocation of the links.

					// Double the physical levels of the node.
					integer newCapacity = 
						2 * capacity - 1;
					link_.reserve(newCapacity);

					// The std::vector's push_back does
					// such an exponential growing 
					// automatically. However, we want to
					// be specific here that the number of
					// levels multiplies by 2, and not
					// by some other constant.
				}

				// Add the new logical level.
				// The default of linking to itself is
				// useful for the sentinel node. The other
				// nodes immediately overwrite the links.
				link_.push_back(Link(this, this));
			}

			integer levels() const
			{
				return link_.size();
			}

		//private:
			class Link
			{
			public:
				Link() = default;

				Link(Node* next_,
					Node* prev_)
				: next()
				{
					next[0] = next_;
					next[1] = prev_;
				}

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
			Data_Node() = delete;
			Data_Node(const Data_Node&) = delete;
			Data_Node(Data_Node&&) = delete;

			Data_Node(
				Key key,
				Value_Class data,
				integer levels = 0)
			: Node(levels)
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
