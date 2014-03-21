// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/ordered_field_concept.h"

namespace Pastel
{

	template <typename Real>
	class Real_Function
	{
	public:
		PASTEL_CONSTEXPR bool Exists = false;

		Real infinity();
		Real nan();
	};

	namespace Real_Concept
	{

		//! A real number.
		/*!
		Each model of Real must specialize and implement 
		the Real_Function class template.
		*/
		class Real
		: public Ordered_Field_Concept::Ordered_Field
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
		};

		//! Returns 1 / that.	
		Real inverse(const Real& that);

		//! Returns the greatest integer <= that.
		Real floor(const Real& that);

		//! Returns the least integer >= that.
		Real ceil(const Real& that);

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
	*/

}

#include "pastel/sys/real_concept.hpp"

#endif
