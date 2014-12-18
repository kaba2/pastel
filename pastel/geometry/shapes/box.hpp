#ifndef PASTELGEOMETRY_BOX_HPP
#define PASTELGEOMETRY_BOX_HPP

#include "pastel/geometry/shapes/box.h"

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N>::Box()
		: position_(0)
		, width_(1)
		, rotation_(N, N)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(integer dimension)
		: position_(ofDimension(dimension), 0)
		, width_(ofDimension(dimension), 1)
		, rotation_(dimension, dimension)
	{
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		const Vector<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		integer dimension,
		const Vector<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		PENSURE_OP(dimension, ==, position.size());
		PENSURE_OP(dimension, ==, width.size());
		PENSURE_OP(dimension, ==, rotation.width());
		PENSURE_OP(dimension, ==, rotation.height());
	}

	template <typename Real, int N>
	Box<Real, N>::~Box()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	integer Box<Real, N>::n() const
	{
		return position_.n();
	}

	template <typename Real, int N>
	void Box<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		position_ = position;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Box<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, int N>
	void Box<Real, N>::setWidth(
		const Vector<Real, N>& width)
	{
		width_ = width;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Box<Real, N>::width() const
	{
		return width_;
	}

	template <typename Real, int N>
	void Box<Real, N>::setRotation(
		const Matrix<Real>& rotation)
	{
		rotation_ = rotation;
	}

	template <typename Real, int N>
	const Matrix<Real>& Box<Real, N>::rotation() const
	{
		return rotation_;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator*=(
		const Real& that)
	{
		width_ *= that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		width_ /= that;

		return *this;
	}

}

#endif
