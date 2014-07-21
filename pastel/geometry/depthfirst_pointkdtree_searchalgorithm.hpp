#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTHFIRST_SEARCHALGORITHM_HPP
#define PASTELGEOMETRY_POINTKDTREE_DEPTHFIRST_SEARCHALGORITHM_HPP

#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"

#include "pastel/sys/mytypes.h"

#include <vector>

namespace Pastel
{

	class DepthFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <typename State>
		class Instance
		{
		public:
			bool nodesLeft() const
			{
				return !stateSet_.empty();
			}

			State nextNode()
			{
				ASSERT(nodesLeft());

				State state = stateSet_.back();
				stateSet_.pop_back();
			
				return state;
			}

			bool breakOnCulling() const
			{
				return false;
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
				stateSet_.emplace_back(state);
			}

			void insertNodes(
				const State& left, 
				const State& right)
			{
				if (left.distance < right.distance)
				{
					stateSet_.emplace_back(right);
					stateSet_.emplace_back(left);
				}
				else
				{
					stateSet_.emplace_back(left);
					stateSet_.emplace_back(right);
				}
			}

		private:
			std::vector<State> stateSet_;
		};
	};

}

#endif
