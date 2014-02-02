#ifndef PASTELSYS_REAL_CONCEPT_HPP
#define PASTELSYS_REAL_CONCEPT_HPP

#include "pastel/sys/real_concept.h"

namespace Pastel
{

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		infinity()
	{
		Real_Function<Real> f;
		// For some reason Visual Studio
		// does not count the member function call 
		// as usage, and reports a warning.
		unused(f);
		return f.infinity();
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		nan()
	{
		Real_Function<Real> f;
		unused(f);
		return f.nan();
	}

}

#endif
