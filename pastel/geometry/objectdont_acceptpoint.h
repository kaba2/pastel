// Description: ObjectDont_AcceptPoint
// Detail: Rejects a given point but accepts otherwise.
// Documentation: acceptpoint.txt

#ifndef PASTEL_OBJECTDONT_ACCEPTPOINT_H
#define PASTEL_OBJECTDONT_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename Type, typename DerefType>
	class ObjectDont_AcceptPoint
	{
	public:
		typedef Type Object;

		explicit ObjectDont_AcceptPoint(
			const DerefType& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Object& that) const
		{
			return that->object() != exception_;
		}

	private:
		DerefType exception_;
	};

}

#endif
