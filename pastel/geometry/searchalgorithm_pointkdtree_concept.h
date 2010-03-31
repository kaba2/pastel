// Description: SearchAlgorithm_PointKdTree concept
// Documentation: searchalgorithm_pointkdtree.txt

#ifndef PASTEL_SEARCHALGORITHM_POINTKDTREE_CONCEPT_HPP
#define PASTEL_SEARCHALGORITHM_POINTKDTREE_CONCEPT_HPP

#include "pastel/sys/keyvalue.h"

#include <vector>

namespace Pastel
{

	class SearchAlgorithm_PointKdTree_Concept
	{
	public:
		template <typename Real, typename Cursor>
		class Instance
		{
		public:
			typedef KeyValue<Real, Cursor> Entry;
 
			//! Returns the next node to visit.
			/*!
			In case there are no more nodes left to visit,
			this should return KeyValue<Real, Cursor>();
			*/
			Entry nextNode();

			//! Whether to terminate when a node is culled.
			/*!
			For example, when the best-first search algorithm
			sees the first node that is culled, it can terminate
			safely, because it chooses the nodes in increasing
			distance. On the contrary, the depth-first search
			algorithm can't deduce anything from an culling event.
			*/
			bool breakOnCulling() const;

			//! Whether a split-node should be searched or not.
			/*!
			For example, if the whole subtree does not contain
			more than 'bucketSize' number of objects, it makes
			more sense to search it with brute-force right-away,
			than to traverse the hierarchy.
			*/
			bool shouldSearchSplitNode(
				const Cursor& cursor, integer bucketSize) const;

			//! Whether to skip the given node.
			/*!
			For example, nodes which have no objects under their
			subtree can be skipped at will.
			*/
			bool skipNode(const Cursor& cursor) const;

			//! Queue a node to be visited later.
			void insertNode(
				const Cursor& cursor, const Real& distance);

			//! Queue two nodes to be visited later.
			void insertNodes(
				const Cursor& left, const Real& leftDistance, 
				const Cursor& right, const Real& rightDistance);
		};
	};

}

#endif
