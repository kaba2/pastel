// Description: Variance of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_VARIANCE_H
#define PASTELMATH_POINT_VARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/vector.h"
#include "pastel/math/point_mean.h"

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
		typename Real, 
		typename Point_Input, 
		typename Locator>
	Vector<Real, Locator::N> pointVariance(
		Point_Input pointSet,
		const Locator& locator,
		bool biased,
		const Vector<Real, Locator::N>& mean);

	//! Returns the point-set variance.
	/*!
	This is a convenience function which calss
	pointVariance(pointSet, locator, biased,
		pointMean(pointSet, locator)).
	*/
	template <
		typename Real, 
		typename Point_Input, 
		typename Locator>
	Vector<Real, Locator::N> pointVariance(
		Point_Input pointSet,
		const Locator& locator,
		bool biased = true)
	{
		return Pastel::pointVariance(pointSet, locator, biased,
			pointMean<Real>(pointSet, locator));
	}

}

#include "pastel/math/point_variance.hpp"

#endif
