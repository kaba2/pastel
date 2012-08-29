// Description: Dont_AcceptPoint class
// Detail: Rejects a point with a given iterator but accepts otherwise.
// Documentation: acceptpoint.txt

#ifndef PASTELGEOMETRY_DONT_ACCEPTPOINT_H
#define PASTELGEOMETRY_DONT_ACCEPTPOINT_H

#include "pastel/geometry/acceptpoint_concept.h"

namespace Pastel
{

	template <typename Type>
	class Dont_AcceptPoint
	{
	public:
		typedef Type Point;

		explicit Dont_AcceptPoint(
			const Point& exception)
			: exception_(exception)
		{
		}

		bool operator()(
			const Point& that) const
		{
			return that != exception_;
		}

	private:
		Point exception_;
	};

	template <typename Type>
	Dont_AcceptPoint<Type> dontAcceptPoint(
		const Type& that)
	{
		return Dont_AcceptPoint<Type>(that);
	}

}

#endif
