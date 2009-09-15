#ifndef PASTEL_BOX_HPP
#define PASTEL_BOX_HPP

#include "pastel/geometry/box.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N>::Box()
		: position_(0)
		, width_(1)
		, rotation_()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(integer dimension)
		: position_(ofDimension(dimension), 0)
		, width_(ofDimension(dimension), 1)
		, rotation_()
	{
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		const Point<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real, N, N>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		integer dimension,
		const Point<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real, N, N>& rotation)
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
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	integer Box<Real, N>::dimension() const
	{
		return position_.dimension();
	}

	template <typename Real, int N>
	void Box<Real, N>::setPosition(
		const Point<Real, N>& position)
	{
		position_ = position;
	}

	template <typename Real, int N>
	const Point<Real, N>& Box<Real, N>::position() const
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
		const Matrix<Real, N, N>& rotation)
	{
		rotation_ = rotation;
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>& Box<Real, N>::rotation() const
	{
		return rotation_;
	}

}

#endif
