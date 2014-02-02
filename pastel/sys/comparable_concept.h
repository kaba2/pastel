// Description: Comparable concept
// Documentation: algebra.txt

#ifndef PASTELSYS_COMPARABLE_CONCEPT_H
#define PASTELSYS_COMPARABLE_CONCEPT_H

namespace Pastel
{

	namespace Comparable_Concept
	{

		class Comparable {};

		//! Returns whether left == right.
		bool operator==(
			const Comparable& left,
			const Comparable& right);
		
		//! Returns whether left != right.
		bool operator!=(
			const Comparable& left,
			const Comparable& right);
	}

}

#endif
