// Description: PointDont_AcceptPoint
// Detail: Rejects a given point but accepts otherwise.
// Documentation: acceptpoint.txt

#ifndef PASTELGEOMETRY_POINTDONT_ACCEPTPOINT_H
#define PASTELGEOMETRY_POINTDONT_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename Type, typename DerefType>
	class PointDont_AcceptPoint
	{
	public:
		typedef Type Point;

		explicit PointDont_AcceptPoint(
			const DerefType& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Point& that) const
		{
			return that->point() != exception_;
		}

	private:
		DerefType exception_;
	};

}

#endif
