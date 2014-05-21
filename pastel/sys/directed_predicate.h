// Description: Directed predicate
// Documentation: derived_predicates.txt

#ifndef PASTELSYS_DIRECTED_PREDICATE_H
#define PASTELSYS_DIRECTED_PREDICATE_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	//! Directed predicate
	/*!
	If Direction == true, Predicate()(left, right).
	otherwise, Predicate()(right, left).
	*/
	template <typename Predicate, bool Direction>
	class Directed_Predicate;

}

#include "pastel/sys/directed_predicate.hpp"

#endif
