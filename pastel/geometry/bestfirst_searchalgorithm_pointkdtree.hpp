#ifndef PASTEL_BESTFIRST_SEARCHALGORITHM_POINTKDTREE_HPP
#define PASTEL_BESTFIRST_SEARCHALGORITHM_POINTKDTREE_HPP

#include "pastel/geometry/bestfirst_searchalgorithm_pointkdtree.h"

#include "pastel/sys/keyvalue.h"

#include <queue>
#include <vector>

namespace Pastel
{

	class BestFirst_SearchAlgorithm_PointKdTree
	{
	public:
		template <typename Real, typename Cursor>
		class Instance
		{
		public:
			typedef KeyValue<Real, Cursor> Entry;
			typedef std::priority_queue<Entry,
				std::vector<Entry>,
				std::greater<Entry> > EntrySet;
 
			Entry nextNode()
			{
				Entry result;

				if (!entrySet_.empty())
				{
					result = entrySet_.top();
					entrySet_.pop();
				}
			
				return result;
			}

			bool breakOnCulling() const
			{
				return true;
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
				entrySet_.push(keyValue(distance, cursor));
			}

			void insertNodes(
				const Cursor& left, const Real& leftDistance, 
				const Cursor& right, const Real& rightDistance)
			{
				entrySet_.push(keyValue(leftDistance, left));
				entrySet_.push(keyValue(rightDistance, right));
			}

		private:
			EntrySet entrySet_;
		};
	};

}

#endif
