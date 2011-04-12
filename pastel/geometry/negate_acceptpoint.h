// Description: Negate_AcceptPoint class
// Detail: Accepts those points rejected by another AcceptPoint model.
// Documentation: acceptpoint.txt

#ifndef PASTEL_NEGATE_ACCEPTPOINT_H
#define PASTEL_NEGATE_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename AcceptPoint>
	class Negate_AcceptPoint
	{
	public:
		typedef typename AcceptPoint::Point Point;

		Negate_AcceptPoint()
			: inner_()
		{
		}

		explicit Negate_AcceptPoint(
			const AcceptPoint& that)
			: inner_(that)
		{
		}

		bool operator()(const Point& that) const
		{
			return !inner_(that);
		}

	private:
		AcceptPoint inner_;
	};

	template <typename AcceptPoint>
	Negate_AcceptPoint<AcceptPoint> negateAcceptPoint(
		const AcceptPoint& that)
	{
		return Negate_AcceptPoint<AcceptPoint>(that);
	}

}

#endif
