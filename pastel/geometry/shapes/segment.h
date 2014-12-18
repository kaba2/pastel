// Description: Line segment

#ifndef PASTELGEOMETRY_SEGMENT_H
#define PASTELGEOMETRY_SEGMENT_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A line segment.
	/*!
	A line segment is described by two endpoints:
	a 'starting point' P and an 'ending point' Q.
	The line segment is then the set of points
	p = P + t * (Q - P)
	with 0 <= t <= 1
	*/
	template <typename Real, int N = Dynamic>
	class Segment
		: boost::multipliable<Segment<Real, N>, Real
		, boost::dividable<Segment<Real, N>, Real
		, boost::addable<Segment<Real, N>, Vector<Real, N>
		, boost::subtractable<Segment<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static PASTEL_CONSTEXPR int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a singular segment at origin.
		Segment();

		//! Constructs a segment using the given points.
		Segment(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end);

		//! Constructs a segment using the given points.
		Segment(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end,
			Topology startTopology,
			Topology endTopology);

		// Used for concept checking.
		~Segment();

		//! Swaps two segments.
		void swap(Segment& that);

		//! Sets the end points of the segment.
		void set(const Vector<Real, N>& start,
			const Vector<Real, N>& end);

		//! Returns the start point of the segment.
		Vector<Real, N>& start();

		//! Returns the start point of the segment.
		const Vector<Real, N>& start() const;

		//! Returns the end point of the segment.
		Vector<Real, N>& end();

		//! Returns the end point of the segment.
		const Vector<Real, N>& end() const;

		//! Sets the topology of the start point.
		void setStartTopology(Topology startTopology);

		//! Returns the topology of the start point.
		Topology startTopology() const;

		//! Sets the topology of the end point.
		void setEndTopology(Topology endTopology);

		//! Returns the topology of the end point.
		Topology endTopology() const;

		Vector<Real, N> at(const Real& t) const;

		//! Translates the segment by the given vector.
		Segment<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the segment backwards by the given vector.
		Segment<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the segment without affecting position.
		Segment<Real, N>& operator*=(const Real& that);

		//! Scales down the segment without affecting position.
		Segment<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> start_;
		Vector<Real, N> end_;
		Topology startTopology_;
		Topology endTopology_;
	};

	using Segment1 = Segment<real, 1>;
	using Segment2 = Segment<real, 2>;
	using Segment3 = Segment<real, 3>;
	using Segment4 = Segment<real, 4>;
	using SegmentD = Segment<real, Dynamic>;

	template <typename Real, int N>
	void swap(
		Segment<Real, N>& left,
		Segment<Real, N>& right);

}

#include "pastel/geometry/shapes/segment.hpp"

#endif
