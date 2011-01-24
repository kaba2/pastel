// Description: Greater-than algorithm
// Documentation: orderrelationalgorithm.txt

#ifndef PASTEL_GREATERTHAN_H
#define PASTEL_GREATERTHAN_H

#include "pastel/sys/orderrelationalgorithm_concept.h"

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
