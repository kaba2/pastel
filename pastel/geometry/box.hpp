#ifndef PASTEL_BOX_HPP
#define PASTEL_BOX_HPP

#include "pastel/geometry/box.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	Box<N, Real>::Box()
		: position_(0)
		, width_(1)
		, rotation_()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Box<N, Real>::Box(integer dimension)
		: position_(ofDimension(dimension), 0)
		, width_(ofDimension(dimension), 1)
		, rotation_()
	{
	}

	template <int N, typename Real>
	Box<N, Real>::Box(
		const Point<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<N, N, Real>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
	}

	template <int N, typename Real>
	Box<N, Real>::Box(
		integer dimension,
		const Point<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<N, N, Real>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		PENSURE_OP(dimension, ==, position.size());
		PENSURE_OP(dimension, ==, width.size());
		PENSURE_OP(dimension, ==, rotation.width());
		PENSURE_OP(dimension, ==, rotation.height());
	}

	template <int N, typename Real>
	Box<N, Real>::~Box()
	{
		BOOST_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <int N, typename Real>
	integer Box<N, Real>::dimension() const
	{
		return position_.dimension();
	}

	template <int N, typename Real>
	void Box<N, Real>::setPosition(
		const Point<Real, N>& position)
	{
		position_ = position;
	}

	template <int N, typename Real>
	const Point<Real, N>& Box<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Box<N, Real>::setWidth(
		const Vector<Real, N>& width)
	{
		width_ = width;
	}

	template <int N, typename Real>
	const Vector<Real, N>& Box<N, Real>::width() const
	{
		return width_;
	}

	template <int N, typename Real>
	void Box<N, Real>::setRotation(
		const Matrix<N, N, Real>& rotation)
	{
		rotation_ = rotation;
	}

	template <int N, typename Real>
	const Matrix<N, N, Real>& Box<N, Real>::rotation() const
	{
		return rotation_;
	}

}

#endif
