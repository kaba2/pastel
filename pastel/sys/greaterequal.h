// Description: Greater-than-or-equal-to predicate
// Documentation: operator_predicates.txt

#ifndef PASTELSYS_GREATEREQUAL_H
#define PASTELSYS_GREATEREQUAL_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	class GreaterEqual
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left >= right;
		}
	};

}

#endif
