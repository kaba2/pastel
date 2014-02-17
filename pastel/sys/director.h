#ifndef PASTELSYS_DIRECTOR_H
#define PASTELSYS_DIRECTOR_H

#include "pastel/sys/step_indicator_concept.h"
#include "pastel/sys/director_concept.h"

namespace Pastel
{

	template <typename Step_Indicator>
	class Step_Indicator_Director
	{
	public:
		Step_Indicator_Director(Step_Indicator indicator)
			: indicator_(indicator)
		{
		}

		template <typename Integer>
		Integer operator()(const Integer& level) const
		{
			return indicator_(level) ? level : (level + 1);
		};

	private:
		Step_Indicator indicator_;
	};
	
	template <typename Step_Indicator>
	Step_Indicator_Director<Step_Indicator> stepIndicatorDirector(
		Step_Indicator indicator)
	{
		return Step_Indicator_Director<Step_Indicator>(indicator);
	}

}

#endif
