// Description: Complement indicator
// Detail: Accepts objects rejected by another indicator.
// Documentation: indicators.txt

#ifndef PASTELSYS_COMPLEMENT_INDICATOR_H
#define PASTELSYS_COMPLEMENT_INDICATOR_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	template <typename Indicator>
	class Complement_Indicator
	{
	public:
		Complement_Indicator()
			: inner_()
		{
		}

		explicit Complement_Indicator(
			const Indicator& that)
			: inner_(that)
		{
		}

		template <typename Type>
		bool operator()(const Type& that) const
		{
			return !inner_(that);
		}

	private:
		Indicator inner_;
	};

	template <typename Indicator>
	Complement_Indicator<Indicator> complementIndicator(
		const Indicator& that)
	{
		return Complement_Indicator<Indicator>(that);
	}

}

#endif
