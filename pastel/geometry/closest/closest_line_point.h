// Description: Closest points between a line and a point

#ifndef PASTELGEOMETRY_CLOSEST_LINE_POINT_H
#define PASTELGEOMETRY_CLOSEST_LINE_POINT_H

#include "pastel/geometry/shape/line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a line and a point.
	/*!
	Returns:
	A parameter t such that 'line.at(t)' and 'point' are closest points.
	*/
	template <typename Real, int N>
	Real closest(
		const Line<Real, N>& line,
		const Vector<Real, N>& point)
	{
		// Parametrize the line by:
		// L(t) = P + tD
		//
		// The closest point C on the line L
		// to the given point X is given
		// by the condition that vector (X - C) must
		// be orthogonal to D.
		//
		// dot(X - C, D) = 0
		// => dot(X - (P + tD), D) = 0
		// => dot(X - P, D) - t * dot(D, D) = 0
		// => t = dot(X - P, D) / dot(D, D)
		//
		// Because D is a unit vector,
		// t = dot(X - P, D)

		return dot(point - line.position(),
			line.direction());
	}

}

#endif
