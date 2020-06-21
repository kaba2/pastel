// Description: Indicator concept

#ifndef PASTELSYS_INDICATOR_CONCEPT_H
#define PASTELSYS_INDICATOR_CONCEPT_H

#include "pastel/sys/function/function_concept.h"
#include <concepts>

namespace Pastel
{

	struct Indicator_Concept
	: Refines<Function_Concept<bool>>
	{
		template <
			typename Type,
			typename Element>
		auto requires_(Type&& t, Element&& e) -> decltype
		(
			//! Returns whether an element is in the true-set.
			/*!
			The true-set is the set of those elements which the
			indicator maps to true. Similarly for the false-set.
			The false-set is the complement of the true-set.
			*/
			conceptCheck(
				Concept::convertsTo<bool>(addConst(t)(e))
			)
		);
	};

	template <typename T, typename Element>
	concept Indicator_Concept_ = 
		requires(T t, Element e) {
		//! Returns whether an element is in the true-set.
		/*!
		The true-set is the set of those elements which the
		indicator maps to true. Similarly for the false-set.
		The false-set is the complement of the true-set.
		*/
		{t(e)} -> std::convertible_to<bool>;
	};

}

#endif
