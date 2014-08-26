// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/ordered_field_concept.h"

#include <type_traits>

namespace Pastel
{

	namespace Real_Concept
	{

		//! A real number.
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

		//! Returns infinity.	
		template <typename Type>
		PASTEL_ENABLE_IF((std::is_same<Type, Real>), Type) 
			infinity();

		//! Returns not-a-number.	
		template <typename Type>
		PASTEL_ENABLE_IF((std::is_same<Type, Real>), Type) 
			nan();

		//! Returns 1 / that.	
		Real inverse(const Real& that);

		//! Returns the greatest integer <= that.
		Real floor(const Real& that);

		//! Returns the least integer >= that.
		Real ceil(const Real& that);

	}

}

#include "pastel/sys/real_concept.hpp"

#endif
