/*!
\file
\brief A class for a line segment.
*/

#ifndef PASTELGEOMETRY_SEGMENT_H
#define PASTELGEOMETRY_SEGMENT_H

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

	template <int N, typename Real>
	class Segment
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a singular segment at origin.
		Segment();

		//! Constructs a segment using the given points.
		Segment(const Point<N, Real>& start,
			const Point<N, Real>& end);

		// Used for concept checking.
		~Segment();

		//! Sets the end points of the segment.
		void set(const Point<N, Real>& start,
			const Point<N, Real>& end);

		//! Returns the start point of the segment.
		Point<N, Real>& start();

		//! Returns the start point of the segment.
		const Point<N, Real>& start() const;

		//! Returns the end point of the segment.
		Point<N, Real>& end();

		//! Returns the end point of the segment.
		const Point<N, Real>& end() const;

		Point<N, Real> at(const Real& t) const;

	private:
		Point<N, Real> start_;
		Point<N, Real> end_;
	};

	typedef Segment<1, real> Segment1;
	typedef Segment<2, real> Segment2;
	typedef Segment<3, real> Segment3;
	typedef Segment<4, real> Segment4;

}

#include "pastel/geometry/segment.hpp"

#endif
