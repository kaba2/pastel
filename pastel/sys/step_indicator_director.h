// Description: Step-indicator director
// Documentation: director.txt

#ifndef PASTELSYS_STEP_INDICATOR_DIRECTOR_H
#define PASTELSYS_STEP_INDICATOR_DIRECTOR_H

#include "pastel/sys/step_indicator_concept.h"
#include "pastel/sys/director_concept.h"

namespace Pastel
{

	template <typename Step_Indicator>
	class Step_Indicator_Director
	{
	public:
		Step_Indicator_Director(const Step_Indicator& indicator)
			: indicator_(indicator)
		{
		}

		template <typename Integer>
		Integer operator()(const Integer& level) const
		{
			// Note that this is branchless.
			return level + !indicator_(level);
		};

	private:
		const Step_Indicator& indicator_;
	};
	
	template <typename Step_Indicator>
	Step_Indicator_Director<Step_Indicator> stepIndicatorDirector(
		const Step_Indicator& indicator)
	{
		return Step_Indicator_Director<Step_Indicator>(indicator);
	}

}

#endif
