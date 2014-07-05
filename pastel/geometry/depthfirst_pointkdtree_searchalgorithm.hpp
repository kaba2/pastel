#ifndef PASTELGEOMETRY_POINTKDTREE_DEPTHFIRST_SEARCHALGORITHM_HPP
#define PASTELGEOMETRY_POINTKDTREE_DEPTHFIRST_SEARCHALGORITHM_HPP

#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/keyvalue.h"

#include <vector>

namespace Pastel
{

	class DepthFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <typename Real, typename Cursor>
		class Instance
		{
		public:
			using Entry = KeyValue<Real, Cursor>;
 
			Entry nextNode()
			{
				Entry result;

				if (!entrySet_.empty())
				{
					result = entrySet_.back();
					entrySet_.pop_back();
				}
			
				return result;
			}

			bool breakOnCulling() const
			{
				return false;
			}

			bool shouldSearchSplitNode(
				const Cursor& cursor, integer bucketSize) const
			{
				return cursor.points() <= bucketSize;
			}

			bool skipNode(const Cursor& cursor) const
			{
				return cursor.empty();
			}

			void insertNode(
				const Cursor& cursor, const Real& distance)
			{
				entrySet_.push_back(keyValue(distance, cursor));
			}

			void insertNodes(
				const Cursor& left, const Real& leftDistance, 
				const Cursor& right, const Real& rightDistance)
			{
				if (leftDistance < rightDistance)
				{
					entrySet_.push_back(keyValue(rightDistance, right));
					entrySet_.push_back(keyValue(leftDistance, left));
				}
				else
				{
					entrySet_.push_back(keyValue(leftDistance, left));
					entrySet_.push_back(keyValue(rightDistance, right));
				}
			}

		private:
			std::vector<Entry> entrySet_;
		};
	};

}

#endif
