// Description: Equal-to predicate
// Documentation: predicates.txt

#ifndef PASTEL_EQUALTO_H
#define PASTEL_EQUALTO_H

#include "pastel/sys/predicate_concept.h"

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
