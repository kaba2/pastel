// Description: Less-than algorithm
// Documentation: relationalgorithm.txt

#ifndef PASTEL_LESSTHAN_H
#define PASTEL_LESSTHAN_H

#include "pastel/sys/relationalgorithm_concept.h"

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
