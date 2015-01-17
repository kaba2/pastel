#ifndef PASTELSYS_ANGLES_HPP
#define PASTELSYS_ANGLES_HPP

#include "pastel/sys/math/angles.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

	template <typename Real>
	Real radiansToDegrees(
		const NoDeduction<Real>& radians)
	{
		return (radians / constantPi<Real>()) * 180;
	}

	template <typename Real>
	Real degreesToRadians(
		const NoDeduction<Real>& degrees)
	{
		return (degrees / 180) * constantPi<Real>();
	}

	template <typename Real>
	Real positiveDegrees(
		const NoDeduction<Real>& degrees)
	{
		if (negative(degrees))
		{
			return degrees + 360;
		}

		return degrees;
	}

	template <typename Real>
	Real positiveRadians(
		const NoDeduction<Real>& radians)
	{
		if (negative(radians))
		{
			return radians + 2 * constantPi<Real>();
		}

		return radians;
	}

	template <typename Real>
	Real ccwAngle(const Vector<Real, 2>& to)
	{
		if (allEqual(to, 0))
		{
			return 0;
		}

		return positiveRadians<Real>(std::atan2(to.y(), to.x()));
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& from,
		const Vector<Real, 2>& to)
	{
		Real deltaAngle = Pastel::ccwAngle(to) - Pastel::ccwAngle(from);
		if (deltaAngle >= 0)
		{
			return deltaAngle;
		}


		return deltaAngle + 2 * constantPi<Real>();
	}

}

#endif
