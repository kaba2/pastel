// Description: Less-than-or-equal-to algorithm
// Documentation: predicate.txt

#ifndef PASTEL_LESSEQUAL_H
#define PASTEL_LESSEQUAL_H

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
