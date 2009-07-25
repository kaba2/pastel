// Description: Quaternion class
// Detail: Allows to work with quaternions

#ifndef PASTEL_QUATERNION_H
#define PASTEL_QUATERNION_H

#include "pastel/sys/mytypes.h"
#include "pastel/math/mathlibrary.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! A quaternion.

	template <typename Real>
	class Quaternion
	{
	public:
		// Using default copy constructor
		// Using default destructor
		// Using default assignment

		Quaternion();
		explicit Quaternion(const Vector<3, Real>& that);
		explicit Quaternion(const Vector<4, Real>& that);
		Quaternion(const Real& x, const Real& y,
			const Real& z, const Real& w);

		void mulVector(Vector<3, Real>& vec) const;

		Quaternion<Real>& operator*=(const Quaternion<Real>& that);
		Quaternion<Real>& operator+=(const Quaternion<Real>& that);
		Quaternion<Real>& operator-=(const Quaternion<Real>& that);

		Quaternion<Real> operator*(const Quaternion<Real>& that) const;
		Quaternion<Real> operator+(const Quaternion<Real>& that) const;
		Quaternion<Real> operator-(const Quaternion<Real>& that) const;

		Quaternion<Real>& operator*=(const Real& that);
		Quaternion<Real>& operator/=(const Real& that);

		Quaternion<Real> operator*(const Real& that) const;
		Quaternion<Real> operator/(const Real& that) const;

		friend Quaternion<Real> operator*(
			const Real& left, const Quaternion<Real>& right)
		{
			Quaternion<Real> result(right);
			result *= left;
			return result;
		}

		void setRotation(const Vector<3, Real>& axis,
			const Real& angle);
		void conjugate();
		void invert();
		Real norm() const;
		Real norm2() const;
		void normalize();

	private:
		Real x_;
		Real y_;
		Real z_;
		Real w_;
	};

}

#include "pastel/math/quaternion.hpp"

#endif
