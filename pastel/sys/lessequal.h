// Description: Less-than-or-equal-to predicate
// Documentation: operator_predicates.txt

#ifndef PASTELSYS_LESSEQUAL_H
#define PASTELSYS_LESSEQUAL_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	class LessEqual
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left <= right;
		}
	};

}

#endif
