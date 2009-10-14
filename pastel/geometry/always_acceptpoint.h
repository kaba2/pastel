// Description: Always_AcceptPoint class
// Detail: Accepts all points.
// Documentation: acceptpoint.txt

#ifndef PASTEL_ALWAYS_ACCEPTPOINT_H
#define PASTEL_ALWAYS_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename Type>
	class Always_AcceptPoint
	{
	public:
		typedef Type Object;		

		bool operator()(const Object&) const
		{
			return true;
		}
	};

}

#endif
