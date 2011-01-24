// Description: Catmull-Rom spline
// Documentation: interpolation.txt

#ifndef PASTEL_CRSPLINE_H
#define PASTEL_CRSPLINE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Evaluates a Catmull-Rom spline.

	/*!
	Given a sequence of points p = (p1, p2, ...),
	a Catmull-Rom spline interpolates
	all of the points, excluding the end points,
	while obtaining C1 joins
	between segments. The segment between
	'startPoint' and 'endPoint' is given
	by the parameter interval [0, 1].
	To interpolate a sequence p, first use
	the point set (p1, p2, p3, p4) with
	time interval [0, 1], then take another
	point set (p2, p3, p4, p5) with time
	interval [0, 1]. Repeat for all points.
	A Catmull-Rom spline is a special case of
	the Kochanek-Bartels spline, see the
	kbSpline() function. Even more generally,
	one such segment is a special case of a
	cubic Hermite curve, see cubicHermite() function.
	*/

	template <
		typename PointType,
		typename Real>
		PointType crSpline(
		const PointType& previousPoint,
		const PointType& startPoint,
		const PointType& endPoint,
		const PointType& nextPoint,
		const Real& time);

	template <typename Vector_Iterator>
	typename std::iterator_traits<Vector_Iterator>::value_type crSpline(
		const RandomAccessIterator_Range<Vector_Iterator>& pointSet,
		const typename Vector_Scalar<typename std::iterator_traits<Vector_Iterator>::value_type>::type& time);

}

#include "pastel/math/crspline.hpp"

#endif
