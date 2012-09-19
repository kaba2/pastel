#ifndef PASTELMATH_AFFINEBIJECTION_HPP
#define PASTELMATH_AFFINEBIJECTION_HPP

#include "pastel/math/affinebijection.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	AffineBijection<Real, N>::AffineBijection()
		: transform_()
		, inverse_()
		, update_(false)
	{
	}

	template <typename Real, int N>
	AffineBijection<Real, N>::AffineBijection(
		const AffineTransformation<Real, N>& that)
		: transform_(that)
		, inverse_()
		, update_(true)
	{
	}

	template <typename Real, int N>
	AffineBijection<Real, N>::AffineBijection(
		const Matrix<Real>& transformation,
		const Vector<Real, N>& translation)
		: transform_(transformation, translation)
		, inverse_()
		, update_(true)
	{
	}

	template <typename Real, int N>
	AffineBijection<Real, N>::~AffineBijection()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void AffineBijection<Real, N>::swap(AffineBijection<Real, N>& that)
	{
		using std::swap;

		swap(transform_, that.transform_);
		swap(inverse_, that.inverse_);
		swap(update_, that.update_);
	}

	template <typename Real, int N>
	AffineBijection<Real, N>& AffineBijection<Real, N>::operator*=(
		const AffineBijection<Real, N>& that)
	{
		transform_ *= that.transform_;
		update_ = true;

		return *this;
	}

	template <typename Real, int N>
	AffineBijection<Real, N> AffineBijection<Real, N>::operator*(
		const AffineBijection<Real, N>& that) const
	{
		AffineBijection<Real, N> result(*this);
		result *= that;

		return result;
	}

	template <typename Real, int N>
	void AffineBijection<Real, N>::setTransform(
		const AffineTransformation<Real, N>& transformation)
	{
		transform_ = transformation;
		update_ = true;
	}

	template <typename Real, int N>
	const AffineTransformation<Real, N>&
		AffineBijection<Real, N>::forward() const
	{
		return transform_;
	}

	template <typename Real, int N>
	const AffineTransformation<Real, N>&
		AffineBijection<Real, N>::inverse() const
	{
		if (update_)
		{
			update();
		}

		return inverse_;
	}

	template <typename Real, int N>
	void AffineBijection<Real, N>::setMatrix(
		const Matrix<Real>& transformation)
	{
		transform_.matrix() = transformation;
		update_ = true;
	}

	template <typename Real, int N>
	const Matrix<Real>& AffineBijection<Real, N>::matrix() const
	{
		return transform_.matrix();
	}

	template <typename Real, int N>
	const Matrix<Real>&
		AffineBijection<Real, N>::inverseMatrix() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.matrix();
	}

	template <typename Real, int N>
	void AffineBijection<Real, N>::setTranslation(
		const Vector<Real, N>& translation)
	{
		transform_.translation() = translation;
		update_ = true;
	}

	template <typename Real, int N>
	const Vector<Real, N>&
		AffineBijection<Real, N>::translation() const
	{
		return transform_.translation();
	}

	template <typename Real, int N>
	const Vector<Real, N>&
		AffineBijection<Real, N>::inverseTranslation() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.translation();
	}

	// Private

	template <typename Real, int N>
	void AffineBijection<Real, N>::update() const
	{
		inverse_ = Pastel::inverse(transform_);
		update_ = false;
	}

}

#endif
