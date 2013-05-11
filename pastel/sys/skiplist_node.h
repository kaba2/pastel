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

		class Node
		{
		public:
			explicit Node(integer levels)
			: link_(levels)
			{
			}

			template <bool Direction>
			Node*& link(integer i)
			{
				return link_[i].next[Direction];
			}

			template <bool Direction>
			Node* link(integer i) const
			{
				return link_[i].next[Direction];
			}

			integer size() const
			{
				return link_.size();
			}

		//private:
			struct Link
			{
				Node* next[2];
			};
			
			std::vector<Link> link_;
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
				Key key,
				Value_Class data)
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

		template <typename Compare, bool Direction>
		class Directed_Compare;

		template <typename Compare>
		class Directed_Compare<Compare, false>
		{
		public:
			template <typename Left, typename Right>
			bool operator()(
				const Left& left,
				const Right& right)
			{
				return Compare()(right, left);
			}
		};

		template <typename Compare>
		class Directed_Compare<Compare, true>
		{
		public:
			template <typename Left, typename Right>
			bool operator()(
				const Left& left,
				const Right& right)
			{
				return Compare()(left, right);
			}
		};

	}

}

#endif
