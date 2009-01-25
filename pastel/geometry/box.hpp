#ifndef PASTELGEOMETRY_BOX_HPP
#define PASTELGEOMETRY_BOX_HPP

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
	}

	template <int N, typename Real>
	Box<N, Real>::Box(
		const Point<N, Real>& position,
		const Vector<N, Real>& width,
		const Matrix<N, N, Real>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
	}

	template <int N, typename Real>
	Box<N, Real>::~Box()
	{
		BOOST_STATIC_ASSERT(N > 0);
	}

	template <int N, typename Real>
	void Box<N, Real>::setPosition(
		const Point<N, Real>& position)
	{
		position_ = position;
	}

	template <int N, typename Real>
	const Point<N, Real>& Box<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void Box<N, Real>::setWidth(
		const Vector<N, Real>& width)
	{
		width_ = width;
	}

	template <int N, typename Real>
	const Vector<N, Real>& Box<N, Real>::width() const
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
