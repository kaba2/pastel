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

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		inverse(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.inverse(that);
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		floor(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.floor(that);
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, Real) 
		ceil(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.ceil(that);
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		zero(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.zero(that);
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		negative(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.negative(that);
	}

	template <typename Real>
	PASTEL_ENABLE_IF_C(Real_Function<Real>::Exists, bool) 
		positive(const Real& that)
	{
		Real_Function<Real> f;
		unused(f);
		return f.positive(that);
	}

}

#endif
