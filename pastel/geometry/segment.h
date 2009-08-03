// Description: Segment class
// Detail: Allows to work with line segments
// Documentation: shapes.txt

#ifndef PASTEL_SEGMENT_H
#define PASTEL_SEGMENT_H

#include "pastel/sys/point.h"

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
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a singular segment at origin.
		Segment();

		//! Constructs a segment using the given points.
		Segment(const Point<Real, N>& start,
			const Point<Real, N>& end);

		// Used for concept checking.
		~Segment();

		//! Sets the end points of the segment.
		void set(const Point<Real, N>& start,
			const Point<Real, N>& end);

		//! Returns the start point of the segment.
		Point<Real, N>& start();

		//! Returns the start point of the segment.
		const Point<Real, N>& start() const;

		//! Returns the end point of the segment.
		Point<Real, N>& end();

		//! Returns the end point of the segment.
		const Point<Real, N>& end() const;

		Point<Real, N> at(const Real& t) const;

	private:
		Point<Real, N> start_;
		Point<Real, N> end_;
	};

	typedef Segment<real, 1> Segment1;
	typedef Segment<real, 2> Segment2;
	typedef Segment<real, 3> Segment3;
	typedef Segment<real, 4> Segment4;

}

#include "pastel/geometry/segment.hpp"

#endif
