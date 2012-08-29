// Description: Function algorithm concept
// Documentation: functions.txt

#ifndef PASTELSYS_FUNCTION_CONCEPT_H
#define PASTELSYS_FUNCTION_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Function_Concept
	{

		class Function
		{
		public:
			//! Returns the value of the function at x.
			UserDefinedType operator()(
				const UserDefinedType& x) const;
		};

	}

}

#endif
