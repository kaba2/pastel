// Description: Not-equal-to predicate
// Documentation: operator_predicates.txt

#ifndef PASTELSYS_NOTEQUALTO_H
#define PASTELSYS_NOTEQUALTO_H

#include "pastel/sys/predicate_concept.h"
#include "pastel/sys/not_predicate.h"

namespace Pastel
{

	using NotEqualTo = Not_Predicate<EqualTo>;

}

#endif
