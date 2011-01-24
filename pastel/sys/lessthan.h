// Description: Less-than algorithm
// Documentation: orderrelationalgorithm.txt

#ifndef PASTEL_LESSTHAN_H
#define PASTEL_LESSTHAN_H

#include "pastel/sys/orderrelationalgorithm_concept.h"

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
