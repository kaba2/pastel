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
		typedef typename AcceptPoint::Object Object;

		bool operator()(const Object& that) const
		{
			return !inner_(that);
		}

	private:
		AcceptPoint inner_;
	};

}

#endif
