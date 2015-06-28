// Description: Step-indicator concept

#ifndef PASTELSYS_STEP_INDICATOR_CONCEPT_H
#define PASTELSYS_STEP_INDICATOR_CONCEPT_H

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/algebra/linearly_ordered_element_concept.h"

namespace Pastel
{

	struct Step_Indicator_Concept
		: Refines<Indicator_Concept>
	{
		//! A step-indicator.
		/*!
		A step-indicator is an indicator f : S --> {0, 1},
		where S is a linearly-ordered-set, such that
		there exists k in S such that

		    f(n) = {0, if n < k
		    	   {1, if otherwise
		*/
		template <
			typename Type,
			typename Element>
		auto requires(Type&& t, Element&& e) -> decltype
		(
			conceptCheck(
				Concept::isModelOf<Linearly_Ordered_Element_Concept>(e)
			)
		);
	};

}

#endif
