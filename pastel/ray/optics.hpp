#ifndef PASTELRAY_OPTICS_HPP
#define PASTELRAY_OPTICS_HPP

#include "pastel/ray/optics.h"

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/math_functions.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real>
	Real cosCriticalAngle2(const Real& fromIndex, const Real& toIndex)
	{
		PENSURE_OP(fromIndex, >=, toIndex);

		const Real indexRatio(toIndex / fromIndex);

		return Real(1) - indexRatio * indexRatio;
	}

	template <typename Real, int N>
	Vector<Real, N> reflect(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal)
	{
		const Real cosFromNormal(dot(from, normal));

		return reflect(from, normal, cosFromNormal);
	}

	template <typename Real, int N>
	Vector<Real, N> reflect(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
		const PASTEL_NO_DEDUCTION(Real)& cosFromNormal)
	{
		return 2 * cosFromNormal * normal - from;
	}

	template <typename Real, int N>
	Vector<Real, N> refract(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
		const PASTEL_NO_DEDUCTION(Real)& fromIndex,
		const PASTEL_NO_DEDUCTION(Real)& toIndex)
	{
		const Real cosFromNormal(
			dot(from, normal));

		return refract(from, normal,
			fromIndex, toIndex, cosFromNormal);
	}

		template <typename Real, int N>
	Vector<Real, N> refract(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
		const PASTEL_NO_DEDUCTION(Real)& fromIndex,
		const PASTEL_NO_DEDUCTION(Real)& toIndex,
		const PASTEL_NO_DEDUCTION(Real)& cosFromNormal)
	{
		// Snell's law:
		// sin(from, normal) * fromIndex = sin(to, normal) * toIndex

		// Solve cos(to, normal) from this equation.
		// =>
		// sin^2(from, normal) * fromIndex^2 =
		// sin^2(to, normal) * toIndex^2
		// =>
		// (1 - cos^2(from, normal)) * fromIndex^2 =
		// (1 - cos^2(to, normal)) * toIndex^2
		// =>
		// (1 - dot^2(from, normal)) * fromIndex^2 =
		// (1 - cos^2(to, normal)) * toIndex^2
		// =>
		// (1 - dot^2(from, normal)) * fromIndex^2 / toIndex^2 - 1 =
		// -cos^2(to, normal)
		// =>
		// cos(to, normal) = sqrt(1 - (1 - dot^2(from, normal)) *
		//                        fromIndex^2 / toIndex^2)

		const Real indexRatio(fromIndex / toIndex);
		const Real cosToNormal(
			std::sqrt(1 - (1 - cosFromNormal * cosFromNormal) *
			indexRatio * indexRatio));

		// Solve a pair of equations for factors u and v.
		// 1) dot(u * from + v * normal, -normal) = cosToNormal
		// 2) dot(u * from + v * normal, u * from + v * normal) = 1
		// with restrictions u <= 0 and v <= 0.

		// Solve v from 1:
		// -u * dot(from, normal) - v * dot(normal, normal) = cosToNormal
		// =>
		// v = -u * cosFromNormal - cosToNormal

		// Now solve u from 2:
		// dot(u * from - u * cosFromNormal * normal - cosToNormal * normal,
		//     u * from - u * cosFromNormal * normal - cosToNormal * normal) = 1
		// =>
		// dot(u * (from - cosFromNormal * normal) - cosToNormal * normal,
		//     u * (from - cosFromNormal * normal) - cosToNormal * normal) = 1

		// Let
		// A = from - cosFromNormal * normal
		// B = cosToNormal * normal
		// =>
		// dot(u * A - B, u * A - B) = 1
		// =>
		// u^2 * dot(A, A) - u * 2 * dot(A, B) + dot(B, B) - 1 = 0
		// dot(A, A) = 1 - 2 * cosFromNormal^2 + cosFromNormal^2
		//           = 1 - cosFromNormal^2
		//           = sinFromNormal^2
		// dot(A, B) = 0
		// dot(B, B) = cosToNormal^2
		// =>
		// u^2 * sinFromNormal^2 + cosToNormal^2 - 1 = 0
		// =>
		// u^2 * sinFromNormal^2 - sinToNormal^2 = 0
		// =>
		// u = sqrt(sinToNormal^2 /
		//          sinFromNormal^2)
		// => (u <= 0)
		// u = -sinToNormal / sinFromNormal
		//   = -indexRatio
		// =>
		// v = indexRatio * cosFromNormal - cosToNormal

		// Results:
		// The refraction direction R is:
		// R = -indexRatio * from +
		//     (indexRatio * cosFromNormal - cosToNormal) * normal

		if (cosFromNormal < 0)
		{
			return Vector<Real, N>(
				-indexRatio * from -
				(indexRatio * cosFromNormal +
				cosToNormal) * normal);
		}

		return Vector<Real, N>(
			-indexRatio * from +
			(indexRatio * cosFromNormal -
			cosToNormal) * normal);
	}

	template <typename Real>
	Real sFresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal)
	{
		const Real sFromFactor(fromIndex * cosFromNormal);
		const Real sToFactor(toIndex * cosToNormal);
		const Real s(
			(sFromFactor - sToFactor) /
			(sFromFactor + sToFactor));

		const Real s2(s * s);

		if (s2 > 1)
		{
			return 1;
		}

		return s2;
	}

	template <typename Real>
	Real pFresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal)
	{
		const Real pFromFactor(fromIndex * cosToNormal);
		const Real pToFactor(toIndex * cosFromNormal);
		const Real p(
			(pFromFactor - pToFactor) /
			(pFromFactor + pToFactor));

		const Real p2(p * p);

		if (p2 > 1)
		{
			return 1;
		}

		return p2;
	}

	template <typename Real>
	Real fresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal)
	{
		return (sFresnelDielectric(fromIndex, toIndex,
			cosFromNormal, cosToNormal) +
			pFresnelDielectric(fromIndex, toIndex,
			cosFromNormal, cosToNormal)) * 0.5;
	}

}

#endif
