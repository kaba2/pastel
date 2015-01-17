// Description: Step-indicator concept

#ifndef PASTELSYS_STEP_INDICATOR_CONCEPT_H
#define PASTELSYS_STEP_INDICATOR_CONCEPT_H

#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/algebra/linearly_ordered_set_concept.h"

namespace Pastel
{

	namespace Step_Indicator_Concept
	{

		//! A step-indicator.
		/*!
		A step-indicator is an indicator f : S --> {0, 1},
		where S is a linearly-ordered-set, such that
		there exists k in S such that

		    f(n) = {0, if n < k
		    	   {1, if otherwise
		*/
		class Step_Indicator
		: public Indicator_Concept::Indicator
		{
		public:
			//! Returns whether an element is >= k.
			template <typename Linearly_Ordered_Set>
			bool operator()(const Linearly_Ordered_Set& element) const;
		};

	}

}

#endif
