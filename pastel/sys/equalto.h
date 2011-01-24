// Description: Equal-to algorithm
// Documentation: relationalgorithm.txt

#ifndef PASTEL_EQUALTO_H
#define PASTEL_EQUALTO_H

#include "pastel/sys/relationalgorithm_concept.h"

namespace Pastel
{

	class EqualTo
	{
	public:
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right) const
		{
			return left == right;
		}
	};

}

#endif
