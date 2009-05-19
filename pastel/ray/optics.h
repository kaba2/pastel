#ifndef PASTEL_OPTICS_H
#define PASTEL_OPTICS_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the critical angle of the total internal reflection.
	/*!
	Preconditions:
	fromIndex >= toIndex

	When moving from a more dense medium to a less dense medium,
	total internal reflection happens after a certain angle.
	This function computes the squared cosine of that angle.
	*/
	template <typename Real>
	Real cosCriticalAngle2(const Real& fromIndex, const Real& toIndex);

	//! Reflects a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(normal) = 1
	dot(from, normal) >= 0
	*/

	template <int N, typename Real>
	Vector<N, Real> reflect(
		const Vector<N, Real>& from,
		const Vector<N, Real>& normal);

	//! Reflects a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(normal) = 1
	dot(from, normal) >= 0
	cosFromNormal = cos(from, normal)
	*/

	template <int N, typename Real>
	Vector<N, Real> reflect(
		const Vector<N, Real>& from,
		const Vector<N, Real>& normal,
		const PASTEL_NO_DEDUCTION(Real)& cosFromNormal);

	//! Refracts a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(from) = 1
	norm(normal) = 1
	dot(from, normal) >= 0
	toIndex != 0
	*/

	template <int N, typename Real>
	Vector<N, Real> refract(
		const Vector<N, Real>& from,
		const Vector<N, Real>& normal,
		const PASTEL_NO_DEDUCTION(Real)& fromIndex,
		const PASTEL_NO_DEDUCTION(Real)& toIndex);

	//! Refracts a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(from) = 1
	norm(normal) = 1
	dot(from, normal) >= 0
	cosFromNormal = cos(from,normal)
	toIndex != 0
	*/

	template <int N, typename Real>
	Vector<N, Real> refract(
		const Vector<N, Real>& from,
		const Vector<N, Real>& normal,
		const PASTEL_NO_DEDUCTION(Real)& fromIndex,
		const PASTEL_NO_DEDUCTION(Real)& toIndex,
		const PASTEL_NO_DEDUCTION(Real)& cosFromNormal);

	//! Calculates the amount of reflected energy from a dielectric.
	/*!
	Preconditions:
	norm(from) = 1
	norm(normal) = 1
	dot(from, normal) >= 0
	to = refract(from, normal, fromIndex, toIndex)
	*/

	template <int N, typename Real>
	Real fresnelDielectric(
		const Vector<N, Real>& from,
		const Vector<N, Real>& normal,
		const Vector<N, Real>& to,
		const PASTEL_NO_DEDUCTION(Real)& fromIndex,
		const PASTEL_NO_DEDUCTION(Real)& toIndex);

	//! Calculates the amount of reflected energy from a dielectric.
	/*!
	Preconditions:
	cosFromNormal e [0, 1]
	cosToNormal e [0, 1]
	Light is s-polarized.
	*/

	template <typename Real>
	Real sFresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal);

	//! Calculates the amount of reflected energy from a dielectric.
	/*!
	Preconditions:
	cosFromNormal e [0, 1]
	cosToNormal e [0, 1]
	Light is p-polarized.
	*/

	template <int N, typename Real>
	Real pFresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal);

	//! Calculates the amount of reflected energy from a dielectric.
	/*!
	Preconditions:
	cosFromNormal e [0, 1]
	cosToNormal e [0, 1]
	Light is unpolarized.
	*/

	template <int N, typename Real>
	Real fresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal);

}

#include "pastel/ray/optics.hpp"

#endif
