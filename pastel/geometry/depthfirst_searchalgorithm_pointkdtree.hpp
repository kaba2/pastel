#ifndef PASTEL_DEPTHFIRST_SEARCHALGORITHM_POINTKDTREE_HPP
#define PASTEL_DEPTHFIRST_SEARCHALGORITHM_POINTKDTREE_HPP

#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"

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
			typedef KeyValue<Real, Cursor> Entry;
 
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
				return cursor.objects() <= bucketSize;
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
