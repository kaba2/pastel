// Description: Director concept
// Documentation: directors.txt

#ifndef PASTELSYS_DIRECTOR_CONCEPT_H
#define PASTELSYS_DIRECTOR_CONCEPT_H

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
		public:
			//! A type of the Integer concept.
			using Integer = UserDefinedType;

			//! Returns f(n).
			Integer operator()(const Integer& n);
		};

	}

}

#endif
