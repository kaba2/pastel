// Description: Real concept
// Documentation: reals.txt

#ifndef PASTEL_REAL_CONCEPT_H
#define PASTEL_REAL_CONCEPT_H

namespace Pastel
{

	template <typename Real>
	class Real_Function
	{
	public:
		static const bool Exists = false;

		Real infinity();
		Real nan();
		Real inverse(const Real& that);
		Real floor(const Real& that);
		Real ceil(const Real& that);
		bool zero(const Real& that);
		bool negative(const Real& that);
		bool positive(const Real& that);
	};

	namespace Real_Concept
	{

		// Each model of Real must specialize and implement 
		// the Real_Function class template.

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
	
	}

	// Interestingly, if the following are uncommented, they
	// will collide with their definitions. I think this
	// may have to do with how Visual Studio handles templates:
	// the Real_Function<Real>::Exists is given a different
	// value in each, because the declaration is treated right
	// away and the definition is treated only at instantiation
	// time.
	/*
	//! Returns the positive infinity.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		infinity();

	//! Returns a Not-A-Number.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		nan();

	//! Returns the multiplicative inverse of 'that'.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		inverse(const Real& that);

	//! Returns the greatest integer less than or equal to 'that'.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		floor(const Real& that);

	//! Returns the smallest integer greater than or equal to 'that'.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		ceil(const Real& that);

	//! Returns if 'that' == 0.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		zero(const Real& that);

	//! Returns if 'that' < 0.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		negative(const Real& that);

	//! Returns if 'that' > 0.
	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		positive(const Real& that);
	*/

}

#include "pastel/sys/real_concept.hpp"

#endif
