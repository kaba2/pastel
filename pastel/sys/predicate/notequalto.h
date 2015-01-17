// Description: Not-equal-to predicate
// Documentation: operator_predicates.txt

#ifndef PASTELSYS_NOTEQUALTO_H
#define PASTELSYS_NOTEQUALTO_H

#include "pastel/sys/predicate/predicate_concept.h"
#include "pastel/sys/predicate/not_predicate.h"

namespace Pastel
{

	using NotEqualTo = Not_Predicate<EqualTo>;

}

#endif
