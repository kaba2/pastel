// Description: SearchAlgorithm_PointKdTree concept
// Documentation: pointkdtree_searchalgorithm.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCHALGORITHM_CONCEPT_HPP
#define PASTELGEOMETRY_POINTKDTREE_SEARCHALGORITHM_CONCEPT_HPP

namespace Pastel
{

	namespace SearchAlgorithm_PointKdTree_Concept
	{

		class SearchAlgorithm_PointKdTree
		{
		public:
			template <typename State>
			class Instance
			{
			public:
				//! Returns whether there are nodes left to visit.
				bool nodesLeft() const;
	 
				//! Returns the next node to visit.
				/*!
				Preconditions:
				nodesLeft()

				In case there are no more nodes left to visit,
				this should return std::pair<Real, Cursor>();
				*/
				State nextNode();

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
				more than 'bucketSize' number of points, it makes
				more sense to search it with brute-force right-away,
				than to traverse the hierarchy.
				*/
				bool shouldSearchSplitNode(
					const State& state, 
					integer bucketSize) const;

				//! Whether to skip the given node.
				/*!
				For example, nodes which have no points under their
				subtree can be skipped at will.
				*/
				bool skipNode(const State& state) const;

				//! Queue a node to be visited later.
				void insertNode(const State& state);

				//! Queue two nodes to be visited later.
				void insertNodes(
					const State& left, 
					const State& right);
			};
		};

	}

}

#endif
