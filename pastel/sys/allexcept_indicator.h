// Description: Co-singleton indicator
// Detail: Rejects a single object.
// Documentation: indicators.txt

#ifndef PASTELSYS_ALLEXCEPT_INDICATOR_H
#define PASTELSYS_ALLEXCEPT_INDICATOR_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	template <typename Type>
	class AllExcept_Indicator
	{
	public:
		explicit AllExcept_Indicator(
			const Type& object)
			: object_(object)
		{
		}

		bool operator()(
			const Type& that) const
		{
			return that != object_;
		}

	private:
		Type object_;
	};

	template <typename Type>
	AllExcept_Indicator<Type> allExceptIndicator(
		const Type& that)
	{
		return AllExcept_Indicator<Type>(that);
	}

}

#endif