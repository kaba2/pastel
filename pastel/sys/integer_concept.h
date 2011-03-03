// Description: Integer concept
// Documentation: integers.txt

#ifndef PASTEL_INTEGER_CONCEPT_H
#define PASTEL_INTEGER_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Integer_Concept
	{

		class Integer
		{
		public:
			Integer();
			Integer(int8 that);
			Integer(uint8 that);

			Integer(int16 that);
			Integer(uint16 that);

			Integer(int32 that);
			Integer(uint32 that);
			
			// Additive operations.

			Integer& operator+=(const Integer& that);
			Integer operator+(const Integer& that) const;

			Integer& operator-=(const Integer& that);
			Integer operator-(const Integer& that) const;

			Integer& operator++();
			Integer& operator--();
			
			Integer operator++(int);
			Integer operator--(int);

			// Multiplicative operations.
			
			Integer& operator*=(const Integer& that);
			Integer operator*(const Integer& that) const;

			Integer& operator/=(const Integer& that);
			Integer operator/(const Integer& that) const;

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

			// Order relations.

			bool operator<(const Integer& that) const;
			bool operator>(const Integer& that) const;
			bool operator<=(const Integer& that) const;
			bool operator>=(const Integer& that) const;

			bool operator==(const Integer& that) const;
			bool operator!=(const Integer& that) const;
		};

		//! Returns that == 0.
		bool zero(const Integer& that);

		//! Returns that < 0.
		bool negative(const Integer& that);

		//! Returns that > 0.
		bool positive(const Integer& that);

	}

}

#endif
