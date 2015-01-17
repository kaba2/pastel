// Description: Integer concept
// Documentation: integers.txt

#ifndef PASTELSYS_INTEGER_CONCEPT_H
#define PASTELSYS_INTEGER_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/algebra/ordered_ring_concept.h"

namespace Pastel
{

	namespace Integer_Concept
	{

		//! An integer.
		class Integer
		: public Ordered_Ring_Concept::Ordered_Ring
		{
		public:
			Integer();
			Integer(int8 that);
			Integer(uint8 that);

			Integer(int16 that);
			Integer(uint16 that);

			Integer(int32 that);
			Integer(uint32 that);
			
			Integer(int64 that);
			Integer(uint64 that);

			// Bitwise operations.

			Integer& operator&=(const Integer& that);
			Integer operator&(const Integer& that) const;

			Integer& operator|=(const Integer& that);
			Integer operator|(const Integer& that) const;

			Integer& operator^=(const Integer& that);
			Integer operator^(const Integer& that) const;

			Integer& operator<<=(const Integer& that);
			Integer operator<<(const Integer& that) const;

			Integer& operator>>=(const Integer& that);
			Integer operator>>(const Integer& that) const;
		};

		//! Returns whether 'that' is even.
		bool even(const Integer& that);

		//! Returns whether 'that' is odd.
		bool odd(const Integer& that);

	}

}

#endif
