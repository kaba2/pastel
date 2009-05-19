#ifndef PASTEL_TRANSFORMATION_HPP
#define PASTEL_TRANSFORMATION_HPP

#include "pastel/math/transformation.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Transformation<N, Real>::Transformation()
		: transform_()
		, inverse_()
		, update_(false)
	{
	}

	template <int N, typename Real>
	Transformation<N, Real>::Transformation(
		const AffineTransformation<N, Real>& that)
		: transform_(that)
		, inverse_()
		, update_(true)
	{
	}

	template <int N, typename Real>
	Transformation<N, Real>::Transformation(
		const Matrix<N, N, Real>& transformation,
		const Vector<N, Real>& translation)
		: transform_(transformation, translation)
		, inverse_()
		, update_(true)
	{
	}

	template <int N, typename Real>
	Transformation<N, Real>::~Transformation()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	void Transformation<N, Real>::swap(Transformation<N, Real>& that)
	{
		using std::swap;
		using std::swap;

		swap(transform_, that.transform_);
		swap(inverse_, that.inverse_);
		swap(update_, that.update_);
	}

	template <int N, typename Real>
	Transformation<N, Real>& Transformation<N, Real>::operator*=(
		const Transformation<N, Real>& that)
	{
		transform_ *= that.transform_;
		update_ = true;

		return *this;
	}

	template <int N, typename Real>
	Transformation<N, Real> Transformation<N, Real>::operator*(
		const Transformation<N, Real>& that) const
	{
		Transformation<N, Real> result(*this);
		result *= that;

		return result;
	}

	template <int N, typename Real>
	void Transformation<N, Real>::setTransform(
		const AffineTransformation<N, Real>& transformation)
	{
		transform_ = transformation;
		update_ = true;
	}

	template <int N, typename Real>
	const AffineTransformation<N, Real>&
		Transformation<N, Real>::affineTransform() const
	{
		return transform_;
	}

	template <int N, typename Real>
	const AffineTransformation<N, Real>&
		Transformation<N, Real>::affineInverse() const
	{
		if (update_)
		{
			update();
		}

		return inverse_;
	}

	template <int N, typename Real>
	void Transformation<N, Real>::setTransform(
		const Matrix<N, N, Real>& transformation)
	{
		transform_.transformation() = transformation;
		update_ = true;
	}

	template <int N, typename Real>
	const Matrix<N, N, Real>& Transformation<N, Real>::transformation() const
	{
		return transform_.transformation();
	}

	template <int N, typename Real>
	const Matrix<N, N, Real>&
		Transformation<N, Real>::inverseTransform() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.transformation();
	}

	template <int N, typename Real>
	void Transformation<N, Real>::setTranslation(
		const Vector<N, Real>& translation)
	{
		transform_.translation() = translation;
		update_ = true;
	}

	template <int N, typename Real>
	const Vector<N, Real>&
		Transformation<N, Real>::translation() const
	{
		return transform_.translation();
	}

	template <int N, typename Real>
	const Vector<N, Real>&
		Transformation<N, Real>::inverseTranslation() const
	{
		if (update_)
		{
			update();
		}

		return inverse_.translation();
	}

	// Private

	template <int N, typename Real>
	void Transformation<N, Real>::update() const
	{
		inverse_ = inverse(transform_);
		update_ = false;
	}

}

#endif
