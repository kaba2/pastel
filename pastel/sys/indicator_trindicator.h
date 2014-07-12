// Description: Indicator trindicator
// DocumentationOf: trindicator.h

#ifndef PASTELSYS_INDICATOR_TRINDICATOR_H
#define PASTELSYS_INDICATOR_TRINDICATOR_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	template <typename Indicator>
	class Indicator_Trindicator
	{
	public:
		Indicator_Trindicator(
			const Indicator& indicator)
			: indicator_(indicator)
		{
		}

		template <typename Type>
		integer operator()(const Type& that) const
		{
			static PASTEL_CONSTEXPR integer ValueSet[] = {1, -1};
			return ValueSet[indicator_(that)];
		}

	private:
		Indicator indicator_;
	};

	template <typename Indicator>
	Indicator_Trindicator<Indicator> indicatorTrindicator(
		const Indicator& indicator)
	{
		return Indicator_Trindicator<Indicator>(indicator);
	}

}

#endif
