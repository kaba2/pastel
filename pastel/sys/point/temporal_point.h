// Description: Temporal point

#ifndef PASTELSYS_TEMPORAL_POINT_H
#define PASTELSYS_TEMPORAL_POINT_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	//! Temporal point
	/*!
	A temporal point is a pairing of a position
	and a time-instant.
	*/
	template <
		typename Point,
		typename Real>
	class Temporal_Point
	{
	public:
		PASTEL_CONCEPT_CHECK(Point, Point_Concept);
		// FIX: Should this be weaker, say Ordered_Set_Concept?
		PASTEL_CONCEPT_CHECK(Real, Real_Concept);

		//! Constructs with default values.
		Temporal_Point()
		: point_()
		, time_()
		{
		}

		//! Constructs with given position and time-instant.
		Temporal_Point(
			const Point& point,
			const Real& time)
		: point_(point)
		, time_(time)
		{
		}

		//! Swaps two temporal points.
		void swap(Temporal_Point& that)
		{
			using std::swap;
			swap(point_, that.point_);
			swap(time_, that.time_);
		}

		//! Returns the position.
		const Point& point() const
		{
			return point_;
		}

		//! Returns the time-instant.
		const Real& time() const
		{
			return time_;
		}

	private:
		Point point_;
		Real time_;
	};

}

#endif
