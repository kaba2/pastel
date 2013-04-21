// Description: Singleton indicator
// Detail: Accepts a single object.
// Documentation: indicators.txt

#ifndef PASTELSYS_SINGLE_INDICATOR_H
#define PASTELSYS_SINGLE_INDICATOR_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	template <typename Type>
	class Single_Indicator
	{
	public:
		explicit Single_Indicator(
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
	Single_Indicator<Type> singleIndicator(
		const Type& object)
	{
		return Single_Indicator<Type>(object);
	}

}

#endif
