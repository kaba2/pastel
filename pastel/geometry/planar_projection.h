// Description: Projections onto a plane
// Documentation: projection.txt

#ifndef PASTEL_PLANAR_PROJECTION_H
#define PASTEL_PLANAR_PROJECTION_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/alignedbox.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <int N, typename Real>
	Point<Real, N> wDivide(const Point<Real, N>& that);

	//! Returns a homogeneous 3D perspective projection matrix.
	/*!
	Preconditions:
	xMin != xMax
	yMin != yMax
	zMin >= 0
	zMax >= 0
	zMin < zMax

	Let the center of projection be at the origin.
	The geometry of the view volume is given by
	the intersection of six half-spaces. These
	are (boundaries in parentheses):

	z >= zMin (near plane)
	z <= zMax (far plane)
	x >= z (xMin / zMin) (left plane)
	x <= z (xMax / zMin) (right plane)
	y >= z (yMin / zMin) (bottom plane)
	y <= z (yMax / zMin) (top plane)

	The view volume will be mapped to the range	[-1, 1]^3.
	*/

	PASTELGEOMETRY void setPerspectiveProjection(
		const AlignedBox2& window,
		const real& zMin,
		const real& zMax,
		Matrix4& matrix);

	//! Returns a homogeneous 3D orthogonal projection matrix.
	/*!
	Preconditions:
	zMin < zMax
	*/
	PASTELGEOMETRY void setOrthogonalProjection(
		const AlignedBox2& window,
		const real& zMin,
		const real& zMax,
		Matrix4& matrix);

	//! Returns a projective transformation relating the given quadrilaterals.
	/*!
	It is assumed that the points in the 'from' point set have
	homogeneous coordinate 1.
	*/

	PASTELGEOMETRY Matrix3 projectiveTransformation(
		const Tuple<Point2, 4>& from,
		const Tuple<Point2, 4>& to);

}

#include "pastel/geometry/planar_projection.hpp"

#endif
