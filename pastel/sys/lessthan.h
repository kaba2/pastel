// Description: Less-than predicate
// Documentation: predicates.txt

#ifndef PASTELSYS_LESSTHAN_H
#define PASTELSYS_LESSTHAN_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	class LessThan
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left < right;
		}
	};

}

#endif
