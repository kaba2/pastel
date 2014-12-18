#ifndef PASTELGEOMETRY_ALIGNEDBOX_TOOLS_HPP
#define PASTELGEOMETRY_ALIGNEDBOX_TOOLS_HPP

#include "pastel/geometry/shapes/alignedbox_tools.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real>& right)
	{
		Vector<Real, 3> aPoint(left.min());
		Vector<Real, 3> bPoint(left.max());
		Vector<Real, 3> cPoint(aPoint[0], aPoint[1], bPoint[2]);
		Vector<Real, 3> dPoint(aPoint[0], bPoint[1], aPoint[2]);
		Vector<Real, 3> ePoint(aPoint[0], bPoint[1], bPoint[2]);
		Vector<Real, 3> fPoint(bPoint[0], aPoint[1], aPoint[2]);
		Vector<Real, 3> gPoint(bPoint[0], aPoint[1], bPoint[2]);
		Vector<Real, 3> hPoint(bPoint[0], bPoint[1], aPoint[2]);

		aPoint = aPoint * right;
		bPoint = bPoint * right;
		cPoint = cPoint * right;
		dPoint = dPoint * right;
		ePoint = ePoint * right;
		fPoint = fPoint * right;
		gPoint = gPoint * right;
		hPoint = hPoint * right;

		AlignedBox<Real, 3> result(
			min(min(min(min(min(min(min(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint),
			max(max(max(max(max(max(max(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint));

		return result;
	}

}

#endif
