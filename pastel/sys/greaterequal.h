// Description: Greater-than-or-equal-to algorithm
// Documentation: relationalgorithm.txt

#ifndef PASTEL_GREATEREQUAL_H
#define PASTEL_GREATEREQUAL_H

#include "pastel/sys/relationalgorithm_concept.h"

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
