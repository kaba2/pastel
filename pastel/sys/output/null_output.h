// Description: Null reporter

#ifndef PASTELSYS_NULL_OUTPUT_H
#define PASTELSYS_NULL_OUTPUT_H

#include "pastel/sys/output/output_concept.h"

namespace Pastel
{

	class Null_Output
	{
	public:
		template <typename... That>
		bool operator()(That&&...) const
		{
			return false;
		}
	};

	inline Null_Output nullOutput()
	{
		return Null_Output();
	}

}

#endif
