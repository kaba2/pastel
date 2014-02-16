// Description: Set concept
// Documentation: algebra.txt

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

namespace Pastel
{

	namespace Set_Concept
	{

		//! A set.
		/*!
		A set is any type whose objects can be
		compared with == and !=.
		*/
		class Set {};

		//! Returns whether left == right.
		bool operator==(
			const Set& left,
			const Set& right);
		
		//! Returns whether left != right.
		bool operator!=(
			const Set& left,
			const Set& right);
	}

}

#endif
