// Description: Predicate indicator
// Detail: Accepts based on comparison to a reference object.
// Documentation: indicators.txt

#ifndef PASTELSYS_PREDICATE_INDICATOR_H
#define PASTELSYS_PREDICATE_INDICATOR_H

#include "pastel/sys/indicator_concept.h"
#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	template <typename Type, typename Predicate>
	class Predicate_Indicator
	{
	public:
		explicit Predicate_Indicator(
			const Type& object)
			: object_(object)
		{
		}

		bool operator()(
			const Type& that) const
		{
			return Predicate()(that, object_);
		}

	private:
		Type object_;
	};

	template <typename Type, typename Predicate>
	Predicate_Indicator<Type, Predicate> predicateIndicator(
		const Type& object, const Predicate& predicate)
	{
		return Predicate_Indicator<Type, Predicate>(object);
	}

}

#endif
