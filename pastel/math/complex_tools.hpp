#ifndef PASTELMATH_COMPLEX_TOOLS_HPP
#define PASTELMATH_COMPLEX_TOOLS_HPP

#include "pastel/math/complex_tools.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <typename Real>
	Complex<Real> inverse(const Complex<Real>& x)
	{
		Real invMod2(inverse(
			x.real() * x.real() + x.imag() * x.imag()));
		return Complex<Real>(
			x.real() * invMod2,
			-x.imag() * invMod2);
	}

	template <typename Real>
	Complex<Real> conjugate(const Complex<Real>& x)
	{
		return Complex<Real>(x.real(), -x.imag());
	}

	template <typename Real>
	Real mod(const Complex<Real>& x)
	{
		const Real& re = x.real();
		const Real& im = x.imag();

		return re * re + im * im;
	}

	template <typename Real>
	Real arg(const Complex<Real>& x)
	{
		const Real& re = x.real();
		const Real& im = x.imag();

		return positiveRadians<Real>(std::atan2(re, im));
	}

	template <typename Real>
	Complex<Real> sin(
		const Complex<Real>& x)
	{
		// sin(x) = (e^(ix) - e^(-ix)) / (2i)

		Complex<Real> ix(x.imag(), -x.real());
		return (exp(ix) - exp(-ix)) / (Complex<Real>(0, 2));
	}

	template <typename Real>
	Complex<Real> cos(
		const Complex<Real>& x)
	{
		// cos(x) = (e^(ix) + e^(-ix)) / 2

		Complex<Real> ix(x.imag(), -x.real());
		return (exp(ix) + exp(-ix)) / 2;
	}

	template <typename Real>
	Complex<Real> tan(
		const Complex<Real>& x)
	{
		// tan(x) = sin(x) / cos(x)

		return sin(x) / cos(x);
	}

	template <typename Real>
	Complex<Real> cot(
		const Complex<Real>& x)
	{
		// cot(x) = cos(x) / sin(x)

		return cos(x) / sin(x);
	}

	template <typename Real>
	Complex<Real> exp(
		const Complex<Real>& x)
	{
		// e^x
		// = e^(Re(x),Im(x))
		// = e^Re(x) * (cos(Im(x)), sin(Im(x)))
		// = (e^Re(x) * cos(Im(x)), e^Re(x) * sin(Im(x)))

		Real expRe(exp(x.real()));
		const Real& im = x.imag();

		return Complex<Real>(
			expRe * cos(im), expRe * sin(im));
	}

	template <typename Real>
	Complex<Real> pow(
		const Complex<Real>& base,
		const Complex<Real>& exponent)
	{
		// x^y = e^(log(x) * y)

		return exp(log(base) * exponent);
	}

	template <typename Real>
	Complex<Real> log(const Complex<Real>& x)
	{
		return Complex<Real>(log(mod(x)), arg(x));
	}

	template <typename Real>
	Complex<Real> polar(const Real& mod, const Real& arg)
	{
		return Complex<Real>(
			mod * std::cos(arg),
			mod * std::sin(arg));
	}

	template <typename Real>
	Complex<Real> sqrt(const Real& x)
	{
		return pow(x, 0.5);
	}

}

#endif
