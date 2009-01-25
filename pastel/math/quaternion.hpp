#ifndef PASTELMATH_QUATERNION_HPP
#define PASTELMATH_QUATERNION_HPP

#include "pastel/math/quaternion.h"

#include "pastel/sys/vector.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <typename Real>
	Quaternion<Real>::Quaternion()
		: x_(0)
		, y_(0)
		, z_(0)
		, w_(0)
	{
	}

	template <typename Real>
	Quaternion<Real>::Quaternion(const Vector<3, Real>& that)
		: x_(0)
		, y_(that[0])
		, z_(that[1])
		, w_(that[2])
	{
	}

	template <typename Real>
	Quaternion<Real>::Quaternion(const Vector<4, Real>& that)
		: x_(that[3])
		, y_(that[0])
		, z_(that[1])
		, w_(that[2])
	{
	}

	template <typename Real>
	Quaternion<Real>::Quaternion(
		const Real& x, const Real& y,
		const Real& z, const Real& w)
		: x_(x)
		, y_(y)
		, z_(z)
		, w_(w)
	{
	}

	template <typename Real>
	Quaternion<Real>& Quaternion<Real>::operator*=(
		const Quaternion<Real>& q)
	{
		Quaternion r(*this);

		x_ = r.x_ * q.x_ - r.y_ * q.y_ - r.z_ * q.z_ - r.w_ * q.w_;
		y_ = r.x_ * q.y_ + r.y_ * q.x_ + r.z_ * q.w_ - r.w_ * q.z_;
		z_ = r.x_ * q.z_ - r.y_ * q.w_ + r.z_ * q.x_ + r.w_ * q.y_;
		w_ = r.x_ * q.w_ + r.y_ * q.z_ - r.z_ * q.y_ + r.w_ * q.x_;

		return *this;
	}

	template <typename Real>
	Quaternion<Real>& Quaternion<Real>::operator+=(
		const Quaternion<Real>& that)
	{
		x_ += that.x_;
		y_ += that.y_;
		z_ += that.z_;
		w_ += that.w_;

		return *this;
	}

	template <typename Real>
	Quaternion<Real>& Quaternion<Real>::operator-=(
		const Quaternion<Real>& that)
	{
		x_ -= that.x_;
		y_ -= that.y_;
		z_ -= that.z_;
		w_ -= that.w_;

		return *this;
	}

	template <typename Real>
	Quaternion<Real> Quaternion<Real>::operator*(
		const Quaternion<Real>& that) const
	{
		Quaternion result(*this);
		result *= that;
		return result;
	}

	template <typename Real>
	Quaternion<Real> Quaternion<Real>::operator+(
		const Quaternion<Real>& that) const
	{
		Quaternion result(*this);
		result += that;
		return result;
	}

	template <typename Real>
	Quaternion<Real> Quaternion<Real>::operator-(
		const Quaternion<Real>& that) const
	{
		Quaternion result(*this);
		result -= that;
		return result;
	}

	template <typename Real>
	Quaternion<Real>& Quaternion<Real>::operator*=(
		const Real& f)
	{
		x_ *= f;
		y_ *= f;
		z_ *= f;
		w_ *= f;

		return *this;
	}

	template <typename Real>
	Quaternion<Real>& Quaternion<Real>::operator/=(
		const Real& that)
	{
		Real invThat(inverse(that));

		x_ *= invThat;
		y_ *= invThat;
		z_ *= invThat;
		w_ *= invThat;

		return *this;
	}

	template <typename Real>
	Quaternion<Real> Quaternion<Real>::operator*(
		const Real& that) const
	{
		Quaternion result(*this);
		result *= that;
		return result;
	}

	template <typename Real>
	Quaternion<Real> Quaternion<Real>::operator/(
		const Real& that) const
	{
		Quaternion result(*this);
		result /= that;
		return result;
	}

	template <typename Real>
	void Quaternion<Real>::setRotation(
		const Vector<3, Real>& axis,
		const Real& angle)
	{
		Vector<3, Real> scaledAxis(axis);
		Real halfAngle = angle * (Real)0.5;

		scaledAxis *= (Real)std::sin(halfAngle);

		x_ = (Real)std::cos(halfAngle);
		y_ = scaledAxis[0];
		z_ = scaledAxis[1];
		w_ = scaledAxis[2];
	}

	template <typename Real>
	void Quaternion<Real>::conjugate()
	{
		y_ = -y_;
		z_ = -z_;
		w_ = -w_;
	}

	template <typename Real>
	void Quaternion<Real>::invert()
	{
		conjugate();
		*this /= norm2();
	}

	template <typename Real>
	Real Quaternion<Real>::norm() const
	{
		return Real(std::sqrt(norm2()));
	}

	template <typename Real>
	Real Quaternion<Real>::norm2() const
	{
		return (x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
	}

	template <typename Real>
	void Quaternion<Real>::normalize()
	{
		Real invNorm(1 / norm());

		x_ *= invNorm;
		y_ *= invNorm;
		z_ *= invNorm;
		w_ *= invNorm;
	}

	template <typename Real>
	void Quaternion<Real>::mulVector(
		Vector<3, Real>& vec) const
	{
		Quaternion q(*this);
		Quaternion r(*this);
		Quaternion u(vec);

		r.invert();
		q *= u;
		q *= r;
	}

}

#endif
