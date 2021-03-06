// Description: ObjectDont_Indicator
// Detail: Rejects a given point but accepts otherwise.
// Documentation: indicator.txt

#ifndef PASTELSYS_OBJECTDONT_INDICATOR_H
#define PASTELSYS_OBJECTDONT_INDICATOR_H

#include "pastel/sys/indicator/indicator_concept.h"

namespace Pastel
{

	template <typename Type, typename DerefType>
	class ObjectDont_Indicator
	{
	public:
		using Point = Type;

		explicit ObjectDont_Indicator(
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
