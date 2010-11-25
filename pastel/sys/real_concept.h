// Description: Real concept
// Documentation: real.txt

#ifndef PASTEL_REAL_CONCEPT_H
#define PASTEL_REAL_CONCEPT_H

namespace Pastel
{

	//! Returns an infinity.
	template <typename Real>
	Real infinity();

	//! Returns a Not-a-Number.
	template <typename Real>
	Real nan();

	namespace Real_Concept
	{

		class Real
		{
		public:
			//! Default-constructed Real's assume value 0.
			Real();

			//! Constructs from an int8.
			Real(int8 that);

			//! Constructs from an uint8.
			Real(uint8 that);

			//! Constructs from an int16.
			Real(int16 that);

			//! Constructs from an uint16.
			Real(uint16 that);

			//! Constructs from an int32.
			Real(int32 that);

			//! Constructs from an uint32.
			Real(uint32 that);

			//! Constructs from a float.
			Real(float that);

			//! Constructs from a double.
			Real(double that);

			// Additive operations.

			Real& operator+=(const Real& that);
			Real operator+(const Real& that) const;

			Real& operator-=(const Real& that);
			Real operator-(const Real& that) const;

			// Multiplicative operations.

			Real& operator*=(const Real& that);
			Real operator*(const Real& that) const;

			Real& operator/=(const Real& that);
			Real operator/(const Real& that) const;

			// Order relations.

			bool operator<(const Real& that) const;
			bool operator>(const Real& that) const;
			bool operator<=(const Real& that) const;
			bool operator>=(const Real& that) const;

			bool operator==(const Real& that) const;
			bool operator!=(const Real& that) const;
		};

		//! Returns the multiplicative inverse of 'that'.
		Real inverse(const Real& that);

		//! Returns the greatest integer less than or equal to 'that'.
		Real floor(const Real& that);

		//! Returns the smallest integer greater than or equal to 'that'.
		Real ceil(const Real& that);

		//! Returns the positive infinity.
		//template <>
		//Real infinity<Real>();

		//! Returns a Not-A-Number.
		//template <>
		//Real nan<Real>();

		//! Returns if 'that' == 0.
		bool zero(const Real& that);

		//! Returns if 'that' < 0.
		bool negative(const Real& that);

		//! Returns if 'that' > 0.
		bool positive(const Real& that);
		
		template <typename NativeReal>
		NativeReal toReal(const Real& that);

	}

}

#include "pastel/sys/real_concept.hpp"

#endif
