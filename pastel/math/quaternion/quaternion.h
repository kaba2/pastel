// Description: Quaternion

#ifndef PASTELMATH_QUATERNION_H
#define PASTELMATH_QUATERNION_H

#include "pastel/sys/mytypes.h"
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
		explicit Quaternion(const Vector<Real, 3>& that);
		explicit Quaternion(const Vector<Real, 4>& that);
		Quaternion(const Real& x, const Real& y,
			const Real& z, const Real& w);

		void mulVector(Vector<Real, 3>& vec) const;

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

		void setRotation(const Vector<Real, 3>& axis,
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

#include "pastel/math/quaternion/quaternion.hpp"

#endif
