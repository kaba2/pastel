// Description: Dont_AcceptPoint class
// Detail: Rejects a point with a given iterator but accepts otherwise.
// Documentation: acceptpoint.txt

#ifndef PASTEL_DONT_ACCEPTPOINT_H
#define PASTEL_DONT_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename Type>
	class Dont_AcceptPoint
	{
	public:
		typedef Type Object;

		explicit Dont_AcceptPoint(
			const Object& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Object& that) const
		{
			return that != exception_;
		}

	private:
		Object exception_;
	};

}

#endif
