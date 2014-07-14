#ifndef PASTELGEOMETRY_POINTKDTREE_BESTFIRST_SEARCHALGORITHM_HPP
#define PASTELGEOMETRY_POINTKDTREE_BESTFIRST_SEARCHALGORITHM_HPP

#include "pastel/geometry/bestfirst_pointkdtree_searchalgorithm.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/keyvalue.h"

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
