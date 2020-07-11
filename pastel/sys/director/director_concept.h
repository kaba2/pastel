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
	template <typename T, typename Integer>
	concept Director_Concept__ = 
		Integer_Concept_<Integer> &&
		requires(T f, Integer n) {
		{f(n)} -> std::convertible_to<Integer>;
	};

	template <typename T, typename Integer>
	concept Director_Concept_ = 
		Director_Concept__<RemoveCvRef<T>, RemoveCvRef<Integer>>;

}

#endif
