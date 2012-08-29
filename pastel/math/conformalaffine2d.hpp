#ifndef PASTELMATH_CONFORMALAFFINE2D_HPP
#define PASTELMATH_CONFORMALAFFINE2D_HPP

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	template <typename Real, int N>
	ConformalAffine2D<Real, N>::ConformalAffine2D(
		const Real& scaling, const Real& rotation)
		: scaling_(scaling)
		, rotation_(rotation)
		, translation_(ofDimension(2), 0)
	{
	}

	template <typename Real, int N>
	ConformalAffine2D<Real, N>::ConformalAffine2D(
		const Real& scaling, const Real& rotation,
		const Vector<Real, N>& translation)
		: scaling_(scaling)
		, rotation_(rotation)
		, translation_(translation)
	{
		PENSURE_OP(translation.size(), ==, 2);
	}

	template <typename Real, int N>
	ConformalAffine2D<Real, N>& ConformalAffine2D<Real, N>::operator=(
		const ConformalAffine2D<Real, N>& that)
	{
		scaling_ = that.scaling_;
		rotation_ = that.rotation_;
		translation_ = that.translation_;

		return *this;
	}

	template <typename Real, int N>
	ConformalAffine2D<Real, N>& ConformalAffine2D<Real, N>::operator*=(
		const ConformalAffine2D<Real, N>& that)
	{
		// Note:
		// It could be that '&that == this' and therefore
		// we should be careful about the order of the operations.

		const Real c = std::cos(rotation_);
		const Real s = std::sin(rotation_);
		const Real xNew = 
			scaling_ * (c * that.translation_[0] - s * that.translation_[1]) + 
			translation_[0];
		const Real yNew = 
			scaling_ * (s * that.translation_[0] + c * that.translation_[1]) + 
			translation_[1];

		scaling_ *= that.scaling_;
		rotation_ += that.rotation_;
		translation_[0] = xNew;
		translation_[1] = yNew;

		return *this;
	}

	template <typename Real, int N>
	Real& ConformalAffine2D<Real, N>::scaling()
	{
		return scaling_;
	}

	template <typename Real, int N>
	const Real& ConformalAffine2D<Real, N>::scaling() const
	{
		return scaling_;
	}

	template <typename Real, int N>
	Real& ConformalAffine2D<Real, N>::rotation()
	{
		return rotation_;
	}

	template <typename Real, int N>
	const Real& ConformalAffine2D<Real, N>::rotation() const
	{
		return rotation_;
	}

	template <typename Real, int N>
	Vector<Real, N>& ConformalAffine2D<Real, N>::translation()
	{
		return translation_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& ConformalAffine2D<Real, N>::translation() const
	{
		return translation_;
	}

}

#endif
