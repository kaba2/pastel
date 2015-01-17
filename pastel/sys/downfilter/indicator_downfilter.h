// Description: Down filter from indicators
// Documentation: downfilter.txt

#ifndef PASTELSYS_INDICATOR_DOWNFILTER_H
#define PASTELSYS_INDICATOR_DOWNFILTER_H

#include "pastel/sys/downfilter_concept.h"
#include "pastel/sys/indicator/indicator_concept.h"

namespace Pastel
{

	template <
		typename Element_Indicator,
		typename DownSet_Indicator>
	class Indicator_DownFilter
	{
	public:
		Indicator_DownFilter(
			const Element_Indicator& elementIndicator,
			const DownSet_Indicator& downSetIndicator)
		: elementIndicator_(elementIndicator)
		, downSetIndicator_(downSetIndicator)
		{
		}

		template <typename Type>
		bool element(Type&& that) const
		{
			return elementIndicator_(std::forward<Type>(that));
		}

		template <typename Type>
		bool downSet(Type&& that) const
		{
			return downSetIndicator_(std::forward<Type>(that));
		}

	private:
		const Element_Indicator& elementIndicator_;
		const DownSet_Indicator& downSetIndicator_;
	};

	template <
		typename Element_Indicator,
		typename DownSet_Indicator>
	Indicator_DownFilter<Element_Indicator, DownSet_Indicator>
		indicatorDownFilter(
			const Element_Indicator& elementIndicator,
			const DownSet_Indicator& downSetIndicator)
	{
		return Indicator_DownFilter<Element_Indicator, DownSet_Indicator>(
			elementIndicator, downSetIndicator);

	}

}

#endif
