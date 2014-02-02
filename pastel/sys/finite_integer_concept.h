// Description: Finite integer concept
// Documentation: integers.txt

#ifndef PASTELSYS_FINITE_INTEGER_CONCEPT_H
#define PASTELSYS_FINITE_INTEGER_CONCEPT_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	namespace Finite_Integer_Concept
	{

		class Finite_Integer
			: public Integer_Concept::Integer
		{
		public:
			//! Returns the binary not of the element.
			Finite_Integer operator~() const;
		};

		//! Returns the number of bits in 'that'.
		integer bits(const Finite_Integer& that);

	}

}

#endif
