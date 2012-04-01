#ifndef PASTEL_TREE_NODE_H
#define PASTEL_TREE_NODE_H

#include "pastel/sys/tree.h"
#include "pastel/sys/possiblyemptymember.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	namespace Tree_Private
	{

		template <typename Data, int N>
		class Node
			: public PossiblyEmptyMember<Data>
		{
		public:
			Node()
				: parent(this)
				, childSet(this)
			{
			}

			explicit Node(Node* sentinel)
				: parent(sentinel)
				, childSet(sentinel)
			{
			}

			Node* parent;
			Tuple<Node*, N> childSet;
		};
	}

}

#endif
