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
			//! Reports 'that'. Returns whether to continue reporting.
			bool operator()(const UserDefinedType& that);
		};
	
	}

}

#endif
