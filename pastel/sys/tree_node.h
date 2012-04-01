#ifndef PASTEL_TREE_NODE_H
#define PASTEL_TREE_NODE_H

#include "pastel/sys/tree.h"
#include "pastel/sys/possiblyemptymember.h"
#include "pastel/sys/tuple.h"

namespace Pastel
{

	namespace Tree_Private
	{

		template <typename Data>
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

			bool sentinel() const
			{
				// The sentinel is the unique node whose
				// children point to itself.
				return childSet[0] == this;
			}

			Node*& childRef(Node* child) const
			{
				ASSERT(child);

				for (integer i = 0;i < 2;++i)
				{
					if (childSet[i] == child)
					{
						return childSet[i];
					}
				}

				const bool gotHere = true;
				ASSERT(!gotHere);

				return 0;
			}

			Node* parent;
			Tuple<Node*, 2> childSet;
		};
	}

}

#endif
