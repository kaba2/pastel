// Description: Variance of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_VARIANCE_H
#define PASTELMATH_POINT_VARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/vector.h"
#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	//! Returns the point-set variance.
	/*!
	Time complexity:
	O(nd)
	where
	n = size(pointSet), and
	d = locator.n().
	*/
	template <
		typename PointSet,
		typename Real = PointSet_Real<PointSet>,
		typename Locator = PointSet_Locator<PointSet>>
	auto pointVariance(
		PointSet pointSet,
		bool biased,
		const Vector<Real, Locator::N>& mean)
		-> Vector<Real, Locator::N>;

	//! Returns the point-set variance.
	/*!
	This is a convenience function which calss
	pointVariance(
		pointSet, biased,
		pointMean(pointSet)).
	*/
	template <
		typename PointSet,
		typename Real = PointSet_Real<PointSet>,
		typename Locator = PointSet_Locator<PointSet>>
	auto pointVariance(
		PointSet pointSet, 
		bool biased = true)
		-> Vector<Real, Locator::N>
	{
		return Pastel::pointVariance(
			pointSet, biased,
			pointMean(pointSet));
	}

}

#include "pastel/math/statistic/point_variance.hpp"

#endif
