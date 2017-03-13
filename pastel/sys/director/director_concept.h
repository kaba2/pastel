// Description: Director concept
// Documentation: director.txt

#ifndef PASTELSYS_DIRECTOR_CONCEPT_H
#define PASTELSYS_DIRECTOR_CONCEPT_H

#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	//! Director
	/*! 
	A director is a function f : ZZ --> ZZ
	such that there exists k in ZZ such that
	 
	    f(n) in [k, n],   if n >= k, and
	    f(n) in (n, k],   if n < k.
	*/
	struct Director_Concept
	{
		template <
			typename Type,
			typename Integer>
		auto requires_(Type&& f, Integer&& n) -> decltype
		(
			conceptCheck(
				// For some reason isModelOf is not defined
				// here. Probably because of circular header inclusions.
				//isModelOf<Integer_Concept>(n),
				f(n)
			)
		);
	};

}

#endif
