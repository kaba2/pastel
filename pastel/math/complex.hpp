#ifndef PASTELMATH_COMPLEX_HPP
#define PASTELMATH_COMPLEX_HPP

#include "pastel/math/complex.h"

namespace Pastel
{

	template <typename Real>
	Complex<Real>::Complex()
		: re_(0)
		, im_(0)
	{
	}

	template <typename Real>
	Complex<Real>::Complex(const Real& re)
		: re_(re)
		, im_(0)
	{
	}

	template <typename Real>
	Complex<Real>::Complex(const Real& re, const Real& im)
		: re_(re)
		, im_(im)
	{
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>::Complex(const Complex<Other>& that)
		: re_(that.re_)
		, im_(that.im_)
	{
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>& Complex<Real>::operator=(
		const Complex<Other>& that)
	{
		re_ = that.re_;
		im_ = that.im_;

		return *this;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>& Complex<Real>::operator+=(
		const Complex<Other>& that)
	{
		re_ += that.re_;
		im_ += that.im_;

		return *this;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>& Complex<Real>::operator-=(
		const Complex<Other>& that)
	{
		re_ -= that.re_;
		im_ -= that.im_;

		return *this;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>& Complex<Real>::operator*=(
		const Complex<Other>& that)
	{
		Complex<Real> copy(*this);

		re_ = copy.re_ * that.re_ - copy.im_ * that.im_;
		im_ = copy.re_ * that.im_ + copy.im_ * that.re_;

		return *this;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real>& Complex<Real>::operator/=(
		const Complex<Other>& that)
	{
		Complex<Real> copy(*this);

		Real invMod(inverse(re_ * re_ + im_ * im_));

		re_ = (copy.re_ * that.re_ + copy.im_ * that.im_) * invMod;
		im_ = (copy.im_ * that.re_ - copy.re_ * that.im_) * invMod;

		return *this;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real> Complex<Real>::operator+(
		const Complex<Other>& that) const
	{
		Complex<Real> result(*this);
		result += that;
		return result;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real> Complex<Real>::operator-(
		const Complex<Other>& that) const
	{
		Complex<Real> result(*this);
		result -= that;
		return result;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real> Complex<Real>::operator*(
		const Complex<Other>& that) const
	{
		Complex<Real> result(*this);
		result *= that;
		return result;
	}

	template <typename Real>
	template <typename Other>
	Complex<Real> Complex<Real>::operator/(
		const Complex<Other>& that) const
	{
		Complex<Real> result(*this);
		result /= that;
		return result;
	}

	template <typename Real>
	Complex<Real>& Complex<Real>::operator+=(
		const Real& that)
	{
		re_ += that;
		return *this;
	}

	template <typename Real>
	Complex<Real>& Complex<Real>::operator-=(
		const Real& that)
	{
		re_ -= that;
		return *this;
	}

	template <typename Real>
	Complex<Real>& Complex<Real>::operator*=(
		const Real& that)
	{
		re_ *= that;
		im_ *= that;

		return *this;
	}

	template <typename Real>
	Complex<Real>& Complex<Real>::operator/=(
		const Real& that)
	{
		Real invThat(inverse(that));

		re_ *= invThat;
		im_ *= invThat;

		return *this;
	}

	template <typename Real>
	Complex<Real> Complex<Real>::operator+(
		const Real& that) const
	{
		Complex<Real> result(*this);
		result += that;
		return result;
	}

	template <typename Real>
	Complex<Real> Complex<Real>::operator-(
		const Real& that) const
	{
		Complex<Real> result(*this);
		result -= that;
		return result;
	}

	template <typename Real>
	Complex<Real> Complex<Real>::operator*(
		const Real& that) const
	{
		Complex<Real> result(*this);
		result *= that;
		return result;
	}

	template <typename Real>
	Complex<Real> Complex<Real>::operator/(
		const Real& that) const
	{
		Complex<Real> result(*this);
		result /= that;
		return result;
	}

	template <typename Real>
	Complex<Real> Complex<Real>::operator-() const
	{
		return Complex<Real>(-re_, -im_);
	}

	template <typename Real>
	Real& Complex<Real>::real()
	{
		return re_;
	}

	template <typename Real>
	const Real& Complex<Real>::real() const
	{
		return re_;
	}

	template <typename Real>
	Real& Complex<Real>::imag()
	{
		return im_;
	}

	template <typename Real>
	const Real& Complex<Real>::imag() const
	{
		return im_;
	}

}

#endif
