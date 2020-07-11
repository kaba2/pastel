// Description: Best-first neighbor searching algorithm for PointKdTree
// Documentation: pointkdtree_searchalgorithm.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_BESTFIRST_SEARCHALGORITHM_H
#define PASTELGEOMETRY_POINTKDTREE_BESTFIRST_SEARCHALGORITHM_H

#include "pastel/geometry/pointkdtree/pointkdtree_searchalgorithm_concept.h"
#include "pastel/sys/mytypes.h"

#include <queue>
#include <vector>

namespace Pastel
{

	class BestFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <typename State>
		class Instance
		{
		public:
			typedef std::priority_queue<
				State,
				std::vector<State>,
				std::greater<State>> StateSet;

			bool nodesLeft() const
			{
				return !stateSet_.empty();
			}
 
			State nextNode()
			{
				ASSERT(nodesLeft());

				State result = stateSet_.top();
				stateSet_.pop();
			
				return result;
			}

			bool breakOnCulling() const
			{
				return true;
			}

			bool shouldSearchSplitNode(
				const State& state, integer bucketSize) const
			{
				return state.cursor.points() <= bucketSize;
			}

			bool skipNode(const State& state) const
			{
				return state.cursor.empty();
			}

			void insertNode(const State& state)
			{
				stateSet_.push(state);
			}

			void insertNodes(
				const State& left,
				const State& right)
			{
				stateSet_.push(left);
				stateSet_.push(right);
			}

		private:
			StateSet stateSet_;
		};
	};

}

#endif
