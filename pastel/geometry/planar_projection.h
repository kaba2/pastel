// Description: Projections onto a plane
// Documentation: projection.txt

#ifndef PASTELGEOMETRY_PLANAR_PROJECTION_H
#define PASTELGEOMETRY_PLANAR_PROJECTION_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/alignedbox.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N> wDivide(const Vector<Real, N>& that);

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

	template <typename Real>
	Matrix<Real> perspectiveProjection(
		const AlignedBox<Real, 2>& window,
		const NoDeduction<Real>& zMin,
		const NoDeduction<Real>& zMax);

	//! Returns a homogeneous 3D orthogonal projection matrix.
	/*!
	Preconditions:
	zMin < zMax
	*/
	template <typename Real>
	Matrix<Real> orthogonalProjection(
		const AlignedBox<Real, 2>& window,
		const NoDeduction<Real>& zMin,
		const NoDeduction<Real>& zMax);

	//! Returns a projective transformation relating the given quadrilaterals.
	/*!
	It is assumed that the points in the 'from' point set have
	homogeneous coordinate 1.
	*/
	template <typename Real>
	Matrix<Real> projectiveTransformation(
		const Tuple<Vector<Real, 2>, 4>& from,
		const Tuple<Vector<Real, 2>, 4>& to);

}

#include "pastel/geometry/planar_projection.hpp"

#endif
