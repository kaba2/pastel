// Description: Null reporter

#ifndef PASTEL_NULL_REPORTER_H
#define PASTEL_NULL_REPORTER_H

#include "pastel/sys/reporter_concept.h"

namespace Pastel
{

	class Null_Reporter
	{
	public:
		template <typename Type>
		void operator()(const Type&) const
		{
		}
	};

}

#endif
