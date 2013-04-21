// Description: Co-singleton indicator
// Detail: Rejects a single object.
// Documentation: indicators.txt

#ifndef PASTELSYS_ALLEXCEPT_INDICATOR_H
#define PASTELSYS_ALLEXCEPT_INDICATOR_H

#include "pastel/geometry/indicator_concept.h"

namespace Pastel
{

	template <typename Type>
	class AllExcept_Indicator
	{
	public:
		typedef Type Point;

		explicit AllExcept_Indicator(
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
	AllExcept_Indicator<Type> dontIndicator(
		const Type& that)
	{
		return AllExcept_Indicator<Type>(that);
	}

}

#endif
