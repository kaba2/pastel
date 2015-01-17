// Description: Equal-to predicate
// Documentation: operator_predicates.txt

#ifndef PASTELSYS_EQUALTO_H
#define PASTELSYS_EQUALTO_H

#include "pastel/sys/predicate/predicate_concept.h"

namespace Pastel
{

	class EqualTo
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left == right;
		}
	};

}

#endif
