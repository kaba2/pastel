// Description: Mean of a point-set
// Documentation: statistics.txt

#ifndef PASTELMATH_POINT_MEAN_H
#define PASTELMATH_POINT_MEAN_H

#include "pastel/sys/input/input_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns the mean of a point set.
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
	Vector<Real, Locator::N> pointMean(
		Point_Input pointSet,
		const Locator& locator);

}

#include "pastel/math/point_mean.hpp"

#endif
