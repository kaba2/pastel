// Description: Statistics from samples
// Detail: pointSetMean, pointSetVariance, etc.
// Documentation: common.txt

#ifndef PASTEL_STATISTICS_H
#define PASTEL_STATISTICS_H

#include "pastel/sys/vector.h"
#include "pastel/sys/forwardrange.h"
#include "pastel/sys/pointpolicy_concept.h"

#include <vector>

namespace Pastel
{

	//! Returns the mean of a point set.
	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> pointSetMean(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy);

	//! Returns the variance along each axis.
	template <typename Point_ConstIterator, typename PointPolicy>
	Vector<typename PointPolicy::Real, PointPolicy::N> 
		pointSetVariance(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const typename PointPolicy::Point& mean,
		const PointPolicy& pointPolicy);

}

#include "pastel/math/statistics.hpp"

#endif
