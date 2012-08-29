// Description: Always_AcceptPoint class
// Detail: Accepts all points.
// Documentation: acceptpoint.txt

#ifndef PASTELGEOMETRY_ALWAYS_ACCEPTPOINT_H
#define PASTELGEOMETRY_ALWAYS_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"
#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Type>
	class Always_AcceptPoint
	{
	public:
		typedef Type Point;		

		bool operator()(const Point&) const
		{
			return true;
		}
	};

	template <typename Real, int N, typename PointPolicy>
	Always_AcceptPoint<
		typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator> 
		alwaysAcceptPoint(const PointKdTree<Real, N, PointPolicy>& tree)
	{
		return Always_AcceptPoint<
			typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>();
	}

}

#endif
