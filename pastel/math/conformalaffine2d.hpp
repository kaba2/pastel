#ifndef PASTELMATH_CONFORMALAFFINE2D_HPP
#define PASTELMATH_CONFORMALAFFINE2D_HPP

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	template <typename Real>
	ConformalAffine2D<Real>::ConformalAffine2D(
		Real scaling, 
		Real rotation)
		: scaling_(std::move(scaling))
		, rotation_(std::move(rotation))
		, translation_(ofDimension(2))
	{
	}

	template <typename Real>
	ConformalAffine2D<Real>::ConformalAffine2D(
		Real scaling, 
		Real rotation,
		Vector<Real> translation)
		: scaling_(std::move(scaling))
		, rotation_(std::move(rotation))
		, translation_(std::move(translation))
	{
		PENSURE_OP(translation.size(), ==, 2);
	}

	template <typename Real>
	ConformalAffine2D<Real>& ConformalAffine2D<Real>::operator=(
		const ConformalAffine2D<Real>& that)
	{
		scaling_ = that.scaling_;
		rotation_ = that.rotation_;
		translation_ = that.translation_;

		return *this;
	}

	template <typename Real>
	ConformalAffine2D<Real>& ConformalAffine2D<Real>::operator*=(
		const ConformalAffine2D<Real>& that)
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

	template <typename Real>
	Real& ConformalAffine2D<Real>::scaling()
	{
		return scaling_;
	}

	template <typename Real>
	const Real& ConformalAffine2D<Real>::scaling() const
	{
		return scaling_;
	}

	template <typename Real>
	Real& ConformalAffine2D<Real>::rotation()
	{
		return rotation_;
	}

	template <typename Real>
	const Real& ConformalAffine2D<Real>::rotation() const
	{
		return rotation_;
	}

	template <typename Real>
	Vector<Real>& ConformalAffine2D<Real>::translation()
	{
		return translation_;
	}

	template <typename Real>
	const Vector<Real>& ConformalAffine2D<Real>::translation() const
	{
		return translation_;
	}

}

#endif
