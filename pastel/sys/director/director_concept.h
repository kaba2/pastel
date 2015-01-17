// Description: Director concept
// Documentation: director.txt

#ifndef PASTELSYS_DIRECTOR_CONCEPT_H
#define PASTELSYS_DIRECTOR_CONCEPT_H

#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	namespace Director_Concept
	{

		//! A director
		/*! 
		A director is a function f : ZZ --> ZZ
		such that there exists k in ZZ such that
		 
		    f(n) in [k, n],   if n >= k, and
		    f(n) in (n, k],   if n < k.
		*/
		class Director
		{
		private:
			//! A type of the Integer concept.
			using Integer = UserDefinedType;

		public:
			//! Returns f(n).
			Integer operator()(const Integer& n);
		};

	}

}

#endif
