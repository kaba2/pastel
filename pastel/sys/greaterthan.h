// Description: Greater-than predicate
// Documentation: predicates.txt

#ifndef PASTELSYS_GREATERTHAN_H
#define PASTELSYS_GREATERTHAN_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	class GreaterThan
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left > right;
		}
	};

}

#endif
