// Description: Concept-checking

#ifndef PASTELSYS_CONCEPT_H
#define PASTELSYS_CONCEPT_H

#include <type_traits>

namespace Pastel
{

	//! List expressions in a concept-check.
	/*!
	This function exists to avoid comma-separated
	expressions from being interpreted as being
	chained by the comma-operator.
	*/
	template <typename... TypeSet>
	void conceptCheck(TypeSet&&... that);

}

#endif
