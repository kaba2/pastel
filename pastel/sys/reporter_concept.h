// Description: Reporter concept
// Documentation: reporters.txt

#ifndef PASTEL_REPORTER_CONCEPT_H
#define PASTEL_REPORTER_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Reporter_Concept
	{

		class Reporter
		{
		public:
			//! Report 'that'.
			void operator()(const UserDefinedType& that);
		};
	
	}

}

#endif
