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
		typedef typename AcceptPoint::Point Object;

		Negate_AcceptPoint()
			: inner_()
		{
		}

		explicit Negate_AcceptPoint(
			const AcceptPoint& that)
			: inner_(that)
		{
		}

		bool operator()(const Object& that) const
		{
			return !inner_(that);
		}

	private:
		AcceptPoint inner_;
	};

}

#endif
