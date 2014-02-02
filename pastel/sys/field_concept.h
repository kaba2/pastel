// Description: Field concept
// Documentation: algebra.txt

#ifndef PASTELSYS_FIELD_CONCEPT_H
#define PASTELSYS_FIELD_CONCEPT_H

#include "pastel/sys/ring_concept.h"

namespace Pastel
{

	namespace Field_Concept
	{

		class Field
		: public Ring_Concept::Ring
		{
		public:
			//! Divides thie element by 'that'.
			Field& operator/=(const Field& that);
		};

		//! Returns left / right.
		Field operator/(
			const Field& left,
			const Field& right);

		//! Returns 1 / that.
		Field inverse(const Field& that);

	}

}

#endif