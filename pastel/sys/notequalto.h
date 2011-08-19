// Description: Not-equal-to predicate
// Documentation: predicates.txt

#ifndef PASTEL_NOTEQUALTO_H
#define PASTEL_NOTEQUALTO_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	class NotEqualTo
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left != right;
		}
	};

}

#endif
