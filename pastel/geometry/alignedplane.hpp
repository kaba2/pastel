#ifndef PASTELGEOMETRY_ALIGNEDPLANE_HPP
#define PASTELGEOMETRY_ALIGNEDPLANE_HPP

#include "pastel/geometry/alignedplane.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <int N, typename Real>
	AlignedPlane<N, Real>::AlignedPlane()
		: position_(0)
		, axis_(0)
	{
	}

	template <int N, typename Real>
	AlignedPlane<N, Real>::AlignedPlane(const Real& position, integer axis)
		: position_(position)
		, axis_(axis)
	{
	}

	template <int N, typename Real>
	AlignedPlane<N, Real>::~AlignedPlane()
	{
		BOOST_STATIC_ASSERT(N == Unbounded || N > 0);
	}

	template <int N, typename Real>
	void AlignedPlane<N, Real>::swap(AlignedPlane<N, Real>& that)
	{
		using std::swap;
		using std::swap;

		swap(position_, that.position_);
		swap(axis_, that.axis_);
	}

	template <int N, typename Real>
	void AlignedPlane<N, Real>::setPosition(const Real& position)
	{
		position_ = position;
	}

	template <int N, typename Real>
	const Real& AlignedPlane<N, Real>::position() const
	{
		return position_;
	}

	template <int N, typename Real>
	void AlignedPlane<N, Real>::setAxis(integer axis)
	{
		axis_ = axis;
	}

	template <int N, typename Real>
	integer AlignedPlane<N, Real>::axis() const
	{
		return axis_;
	}

}

#endif
