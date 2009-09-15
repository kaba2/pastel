#ifndef PASTEL_TRANSFORMATION_HPP
#define PASTEL_TRANSFORMATION_HPP

#include "pastel/math/transformation.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Transformation<Real, N>::Transformation()
		: transform_()
		, inverse_()
		, update_(false)
	{
	}

	template <typename Real, int N>
	Transformation<Real, N>::Transformation(
		const AffineTransformation<Real, N>& that)
		: transform_(that)
		, inverse_()
		, update_(true)
	{
	}

	template <typename Real, int N>
	Transformation<Real, N>::Transformation(
		const Matrix<Real, N, N>& transformation,
		const Vector<Real, N>& translation)
		: transform_(transformation, translation)
		, inverse_()
		, update_(true)
	{
	}

	template <typename Real, int N>
	Transformation<Real, N>::~Transformation()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Transformation<Real, N>::swap(Transformation<Real, N>& that)
	{
		using std::swap;
		using std::swap;

		swap(transform_, that.transform_);
		swap(inverse_, that.inverse_);
		swap(update_, that.update_);
	}

	template <typename Real, int N>
	Transformation<Real, N>& Transformation<Real, N>::operator*=(
		const Transformation<Real, N>& that)
	{
		transform_ *= that.transform_;
		update_ = true;

		return *this;
	}

	template <typename Real, int N>
	Transformation<Real, N> Transformation<Real, N>::operator*(
		const Transformation<Real, N>& that) const
	{
		Transformation<Real, N> result(*this);
		result *= that;

		return result;
	}

	template <typename Real, int N>
	void Transformation<Real, N>::setTransform(
		const AffineTransformation<Real, N>& transformation)
	{
		transform_ = transformation;
		update_ = true;
	}

	template <typename Real, int N>
	const AffineTransformation<Real, N>&
		Transformation<Real, N>::affineTransform() const
	{
		return transform_;
	}

	template <typename Real, int N>
	const AffineTransformation<Real, N>&
		Transformation<Real, N>::affineInverse() const
	{
		if (update_)
		{
			update();
		}

		return inverse_;
	}

	template <typename Real, int N>
	void Transformation<Real, N>::setTransform(
		const Matrix<Real, N, N>& transformation)
	{
		transform_.transformation() = transformation;
		update_ = true;
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>& Transformation<Real, N>::transformation() const
	{
		return transform_.transformation();
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>&
		Transformation<Real, N>::inverseTransform() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.transformation();
	}

	template <typename Real, int N>
	void Transformation<Real, N>::setTranslation(
		const Vector<Real, N>& translation)
	{
		transform_.translation() = translation;
		update_ = true;
	}

	template <typename Real, int N>
	const Vector<Real, N>&
		Transformation<Real, N>::translation() const
	{
		return transform_.translation();
	}

	template <typename Real, int N>
	const Vector<Real, N>&
		Transformation<Real, N>::inverseTranslation() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.translation();
	}

	// Private

	template <typename Real, int N>
	void Transformation<Real, N>::update() const
	{
		inverse_ = inverse(transform_);
		update_ = false;
	}

}

#endif
