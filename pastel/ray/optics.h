// Description: Optical laws
// Documentation: pastelray.txt

#ifndef PASTELRAY_OPTICS_H
#define PASTELRAY_OPTICS_H

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

	template <typename Real, int N>
	Vector<Real, N> reflect(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal);

	//! Reflects a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(normal) = 1
	dot(from, normal) >= 0
	cosFromNormal = cos(from, normal)
	*/

	template <typename Real, int N>
	Vector<Real, N> reflect(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
		const PASTEL_NO_DEDUCTION(Real)& cosFromNormal);

	//! Refracts a vector w.r.t. the normal of a hyperplane.
	/*!
	Preconditions:
	norm(from) = 1
	norm(normal) = 1
	dot(from, normal) >= 0
	toIndex != 0
	*/

	template <typename Real, int N>
	Vector<Real, N> refract(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
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

	template <typename Real, int N>
	Vector<Real, N> refract(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
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

	template <typename Real, int N>
	Real fresnelDielectric(
		const Vector<Real, N>& from,
		const Vector<Real, N>& normal,
		const Vector<Real, N>& to,
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

	template <typename Real, int N>
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

	template <typename Real, int N>
	Real fresnelDielectric(
		const Real& fromIndex,
		const Real& toIndex,
		const Real& cosFromNormal,
		const Real& cosToNormal);

}

#include "pastel/ray/optics.hpp"

#endif
