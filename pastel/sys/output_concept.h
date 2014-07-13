// Description: Output concept
// Documentation: outputs.txt

#ifndef PASTELSYS_OUTPUT_CONCEPT_H
#define PASTELSYS_OUTPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Output_Concept
	{

		class Output
		{
		public:
			//! Reports 'that', and returns whether it was accepted.
			bool operator()(const UserDefinedType& that);
		};
	
	}

}

#endif
